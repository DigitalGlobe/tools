/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS transform implementation for C++ libLAS 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#include <liblas/transform.hpp>
#include <liblas/exception.hpp>
#include <liblas/header.hpp>
// boost
#include <boost/concept_check.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4100) // unreferenced formal param
#endif
#include <boost/algorithm/string/erase.hpp>
#ifdef _MSC_VER
# pragma warning(pop)
#endif

// std
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>

#ifdef HAVE_GDAL
#include <gdal.h>
#include <ogr_spatialref.h>
#endif

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

namespace liblas { 
    

#ifdef HAVE_GDAL
    struct OGRSpatialReferenceDeleter
    {
       template <typename T>
       void operator()(T* ptr)
       {
           ::OSRDestroySpatialReference(ptr);
       }
    };

    struct OSRTransformDeleter
    {
       template <typename T>
       void operator()(T* ptr)
       {
           ::OCTDestroyCoordinateTransformation(ptr);
       }
    };


    struct GDALSourceDeleter
    {
       template <typename T>
       void operator()(T* ptr)
       {
           ::GDALClose(ptr);
       }
    };


#endif


ReprojectionTransform::ReprojectionTransform(const SpatialReference& inSRS, const SpatialReference& outSRS)
    : m_new_header(0)
{
    Initialize(inSRS, outSRS);
}

ReprojectionTransform::ReprojectionTransform(
    const SpatialReference& inSRS, 
    const SpatialReference& outSRS,
    const Header* new_header)
    : m_new_header(new_header)
{
    Initialize(inSRS, outSRS);
}

void ReprojectionTransform::Initialize(const SpatialReference& inSRS, const SpatialReference& outSRS)
{
#ifdef HAVE_GDAL
    
    m_in_ref_ptr = ReferencePtr(OSRNewSpatialReference(0), OGRSpatialReferenceDeleter());
    m_out_ref_ptr = ReferencePtr(OSRNewSpatialReference(0), OGRSpatialReferenceDeleter());
    
    int result = OSRSetFromUserInput(m_in_ref_ptr.get(), inSRS.GetWKT(liblas::SpatialReference::eCompoundOK).c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import input spatial reference for ReprojectionTransform:: " 
            << CPLGetLastErrorMsg() << " code: " << result 
            << "wkt: '" << inSRS.GetWKT() << "'";
        throw std::runtime_error(msg.str());
    }
    
    result = OSRSetFromUserInput(m_out_ref_ptr.get(), outSRS.GetWKT(liblas::SpatialReference::eCompoundOK).c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import output spatial reference for ReprojectionTransform:: " 
            << CPLGetLastErrorMsg() << " code: " << result 
            << "wkt: '" << outSRS.GetWKT() << "'";
        std::string message(msg.str());
        throw std::runtime_error(message);
    }
    m_transform_ptr = TransformPtr(OCTNewCoordinateTransformation( m_in_ref_ptr.get(), m_out_ref_ptr.get()), OSRTransformDeleter());
#else

    boost::ignore_unused_variable_warning(inSRS);
    boost::ignore_unused_variable_warning(outSRS);
#endif
}

ReprojectionTransform::~ReprojectionTransform()
{

}


bool ReprojectionTransform::transform(Point& point)
{
#ifdef HAVE_GDAL
    
    int ret = 0;
    double x = point.GetX();
    double y = point.GetY();
    double z = point.GetZ();

    ret = OCTTransform(m_transform_ptr.get(), 1, &x, &y, &z);    
    if (!ret)
    {
        std::ostringstream msg; 
        msg << "Could not project point for ReprojectionTransform::" << CPLGetLastErrorMsg() << ret;
        throw std::runtime_error(msg.str());
    }
    
    if (this->ModifiesHeader()) 
    {
        if (m_new_header)
            point.SetHeader(m_new_header);
        else 
        {
            // FIXME? 
        }
    }

    point.SetX(x);
    point.SetY(y);
    point.SetZ(z);
    
    if (detail::compare_distance(point.GetRawX(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawX(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("X scale and offset combination is insufficient to represent the data");
    }

    if (detail::compare_distance(point.GetRawY(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawY(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("Y scale and offset combination is insufficient to represent the data");
    }    

    if (detail::compare_distance(point.GetRawZ(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawZ(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("Z scale and offset combination is insufficient to represent the data");
    }        

    return true;
#else
    boost::ignore_unused_variable_warning(point);
    return true;
#endif
}



TranslationTransform::TranslationTransform(std::string const& expression)
{
    if (expression.size() == 0) 
        throw std::runtime_error("no expression was given to TranslationTransform");
    
    boost::char_separator<char> sep_space(" ");

    tokenizer dimensions(expression, sep_space);
    for (tokenizer::iterator t = dimensions.begin(); t != dimensions.end(); ++t) {
        std::string const& s = *t;
        
        operation op = GetOperation(s);
        operations.push_back(op);
    }
    
}

TranslationTransform::operation TranslationTransform::GetOperation(std::string const& expr)
{

    std::string x("x");
    std::string y("y");
    std::string z("z");
    std::string star("*");
    std::string divide("/");
    std::string plus("+");
    std::string minus("-");

    if (expr.find(x) == std::string::npos &&
        expr.find(y) == std::string::npos &&
        expr.find(z) == std::string::npos)
        throw liblas::invalid_expression("expression is invalid -- use x, y, or z to define a dimension.  No 'x', 'y', or 'z' was found");

    operation output("X");
    
    
    std::string expression(expr);
    boost::erase_all(expression, " "); // Get rid of any spaces in the expression chunk


    std::size_t found_x = expression.find(x);
    std::size_t found_y = expression.find(y);
    std::size_t found_z = expression.find(z);
    std::size_t found_star = expression.find(star);
    std::size_t found_divide = expression.find(divide);
    std::size_t found_plus = expression.find(plus);
    std::size_t found_minus = expression.find(minus);
    
    // if they gave something like 'xyz*34' it's invalid
    if (found_x != std::string::npos &&
        found_y != std::string::npos &&
        found_z != std::string::npos)
        throw liblas::invalid_expression("expression is invalid");
    
    std::string::size_type op_pos=std::string::npos;
    if (found_x != std::string::npos)
    {
        output = operation("X");
        op_pos = expression.find_last_of(x) + 1;
    }

    if (found_y != std::string::npos)
    {
        output = operation("Y");
        op_pos = expression.find_last_of(y) + 1;
    }

    if (found_z != std::string::npos)
    {
        output = operation("Z");
        op_pos = expression.find_last_of(z) + 1;
    }
    
    if (op_pos == std::string::npos) 
    {
        std::ostringstream oss;
        oss << "Expression '" << expression << "' does not have 'x', 'y', or 'z' to denote fields";
        throw liblas::invalid_expression(oss.str());
    }
    
    std::string::size_type data_pos = std::string::npos;
    if (found_star != std::string::npos) 
    {
        output.oper = eOPER_MULTIPLY;
        data_pos = expression.find_last_of(star) + 1;
    }

    if (found_divide != std::string::npos) 
    {
        output.oper = eOPER_DIVIDE;
        data_pos = expression.find_last_of(divide) + 1;
    }

    if (found_plus != std::string::npos) 
    {
        output.oper = eOPER_ADD;
        data_pos = expression.find_last_of(plus) + 1;
    }
    if (found_minus != std::string::npos) 
    {
        output.oper = eOPER_SUBTRACT;
        data_pos = expression.find_last_of(minus) + 1;
    }

    if (data_pos == std::string::npos) 
    {
        std::ostringstream oss;
        oss << "Expression '" << expression << "' does not have '*', '/', '+', or '-' to denote operations";
        throw liblas::invalid_expression(oss.str());
    }
    
    std::string out;
    out = expression.substr(data_pos, expression.size());
    
    double value =  boost::lexical_cast<double>(out);    
    output.expression = expression;
    output.value = value;
    
    return output;
            
}
bool TranslationTransform::transform(Point& point)
{
    for(std::vector<TranslationTransform::operation>::const_iterator op = operations.begin();
        op != operations.end();
        op++) 
    {

        switch (op->oper) 
            {
                case eOPER_MULTIPLY:
                    if (!op->dimension.compare("X")) 
                    {
                        point.SetX(point.GetX() * op->value);
                    }
                    if (!op->dimension.compare("Y")) 
                    {
                        point.SetY(point.GetY() * op->value);
                    }
                    if (!op->dimension.compare("Z")) 
                    {
                        point.SetZ(point.GetZ() * op->value);
                    }
                    break;
                case eOPER_DIVIDE:
                    if (!op->dimension.compare("X")) 
                    {
                        point.SetX(point.GetX() / op->value);
                    }
                    if (!op->dimension.compare("Y")) 
                    {
                        point.SetY(point.GetY() / op->value);
                    }
                    if (!op->dimension.compare("Z")) 
                    {
                        point.SetZ(point.GetZ() / op->value);
                    }
                    break;
                case eOPER_ADD:
                    if (!op->dimension.compare("X")) 
                    {
                        point.SetX(point.GetX() + op->value);
                    }
                    if (!op->dimension.compare("Y")) 
                    {
                        point.SetY(point.GetY() + op->value);
                    }
                    if (!op->dimension.compare("Z")) 
                    {
                        point.SetZ(point.GetZ() + op->value);
                    }

                    break;
                case eOPER_SUBTRACT:
                    if (!op->dimension.compare("X")) 
                    {
                        point.SetX(point.GetX() - op->value);
                    }
                    if (!op->dimension.compare("Y")) 
                    {
                        point.SetY(point.GetY() - op->value);
                    }
                    if (!op->dimension.compare("Z")) 
                    {
                        point.SetZ(point.GetZ() - op->value);
                    }
                    break;

                default:
                    std::ostringstream oss;
                    oss << "Unhandled expression operation id " << static_cast<int32_t>(op->oper);
                    throw std::runtime_error(oss.str());
            }

    if (detail::compare_distance(point.GetRawX(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawX(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("X scale and offset combination of this file is insufficient to represent the data given the expression ");
    }

    if (detail::compare_distance(point.GetRawY(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawY(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("Y scale and offset combination of this file is insufficient to represent the data given the expression");
    }    

    if (detail::compare_distance(point.GetRawZ(), (std::numeric_limits<int32_t>::max)()) ||
        detail::compare_distance(point.GetRawZ(), (std::numeric_limits<int32_t>::min)())) {
        throw std::domain_error("Z scale and offset combination of this file is insufficient to represent the data given the expression");
    }   

    }
    return true;
}


TranslationTransform::~TranslationTransform()
{
}

#ifdef HAVE_GDAL
void CPL_STDCALL ColorFetchingTransformGDALErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
    std::ostringstream oss;
    
    if (eErrClass == CE_Failure || eErrClass == CE_Fatal) {
        oss <<"GDAL Failure number=" << err_no << ": " << msg;
        throw std::runtime_error(oss.str());
    } else {
        return;
    }
}
#endif

ColorFetchingTransform::ColorFetchingTransform(
    std::string const& datasource, 
    std::vector<uint32_t> bands
)
    : m_new_header(0)
    , m_ds(DataSourcePtr())
    , m_datasource(datasource)
    , m_bands(bands)
    , m_scale(0)
{
    Initialize();
}

ColorFetchingTransform::ColorFetchingTransform(
    std::string const& datasource, 
    std::vector<uint32_t> bands,
    Header const* header
)
    : m_new_header(header)
    , m_ds(DataSourcePtr())
    , m_datasource(datasource)
    , m_bands(bands)
    , m_scale(0)
{
    Initialize();
}

void ColorFetchingTransform::Initialize()
{
#ifdef HAVE_GDAL
    GDALAllRegister();


    CPLPopErrorHandler();
    CPLPushErrorHandler(ColorFetchingTransformGDALErrorHandler);

    m_ds = DataSourcePtr(GDALOpen( m_datasource.c_str(), GA_ReadOnly ), GDALSourceDeleter());

    // Assume the first three bands are RGB, and not get any more if the 
    // user did not supply any bands 
    if( m_bands.size() == 0 )
    {
        for( int32_t i = 0; i < GDALGetRasterCount( m_ds.get() ); i++ )
        {
            if (i > 3) break;  
            m_bands.push_back( i+1 );
        }
    }

    m_forward_transform.assign(0.0);
    m_inverse_transform.assign(0.0);
    
    if (GDALGetGeoTransform( m_ds.get(), &(m_forward_transform.front()) ) != CE_None )
    {
        throw std::runtime_error("unable to fetch forward geotransform for raster!");
    }

    if (!GDALInvGeoTransform( &(m_forward_transform.front()), &(m_inverse_transform.front())))
    {
        throw std::runtime_error("unable to fetch inverse geotransform for raster!");
    }
    
#endif  
}

bool ColorFetchingTransform::transform(Point& point)
{
#ifdef HAVE_GDAL
    
    int32_t pixel = 0;
    int32_t line = 0;
    
    double x = point.GetX();
    double y = point.GetY();

    if (m_new_header) 
    {
        point.SetHeader(m_new_header);
    }
    
    pixel = (int32_t) std::floor(
        m_inverse_transform[0] 
        + m_inverse_transform[1] * x
        + m_inverse_transform[2] * y );
    line = (int32_t) std::floor(
        m_inverse_transform[3] 
        + m_inverse_transform[4] * x
        + m_inverse_transform[5] * y );
 
    if( pixel < 0 || line < 0 
        || pixel >= GDALGetRasterXSize( m_ds.get() )
        || line  >= GDALGetRasterYSize( m_ds.get() )
        )
    {
        // The x, y is not coincident with this raster, we'll leave whatever
        // color value might be there alone.
        return true;
    }
    
    boost::array<double, 2> pix;
    boost::array<liblas::Color::value_type, 3> color;
    color.assign(0);
    
    for( std::vector<int32_t>::size_type i = 0; 
         i < m_bands.size(); i++ )
         {
             GDALRasterBandH hBand = GDALGetRasterBand( m_ds.get(), m_bands[i] );
             if (hBand == NULL) 
             {
                 continue;
             }
             if( GDALRasterIO( hBand, GF_Read, pixel, line, 1, 1, 
                               &pix[0], 1, 1, GDT_CFloat64, 0, 0) == CE_None )
             {
                 color[i] = static_cast<liblas::Color::value_type>(pix[0]);
                 if (m_scale) {
                     color[i] = color[i] * static_cast<liblas::Color::value_type>(m_scale);
                 }
             }             
         }

     point.SetColor(Color(color));
 
    return true;
#else
    boost::ignore_unused_variable_warning(point);
    return true;
#endif
}
ColorFetchingTransform::~ColorFetchingTransform()
{
#ifdef HAVE_GDAL
    CPLPopErrorHandler();
#endif
}

} // namespace liblas
