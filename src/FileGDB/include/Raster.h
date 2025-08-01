//
// Raster.h
//

/*
   Copyright © 2025 Esri

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   A local copy of the license and additional notices are located with the
   source distribution at:

   http://github.com/Esri/file-geodatabase-api/FileGDB_API_1.5.1
*/

#pragma once

#include <string>

#ifndef EXPORT_FILEGDB_API
# if defined __linux__ || defined __APPLE__
#  define EXT_FILEGDB_API
# else
#  define EXT_FILEGDB_API _declspec(dllimport)
# endif
#else
# if defined __linux__ || defined __APPLE__
#  define EXT_FILEGDB_API __attribute__((visibility("default")))
# else
#  define EXT_FILEGDB_API _declspec(dllexport)
# endif
#endif

#include "FileGDBCore.h"

namespace FileGDBAPI
{

class ByteArray;
class Row;

/// Provides read and write access to raster data.
class EXT_FILEGDB_API Raster
{
public:

  /// @name Data access
  //@{
  /// TODO: Write description.
  /// @param[out]   format TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError GetFormat(std::wstring& format);

  /// TODO: Write description.
  /// @param[in]    format TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError SetFormat(const std::wstring& format);

  /// TODO: Write description.
  /// @param[out]   bytes TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError GetBytes(ByteArray& bytes);

  /// TODO: Write description.
  /// @param[in]    bytes TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError SetBytes(const ByteArray& bytes);

  /// TODO: Write description.
  /// @param[out]   attributes TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError GetAttributes(Row attributes);

  /// TODO: Write description.
  /// @param[in]    attributes TODO
  /// @returns      Error code integer indicating whether the method finished successfully.
  //fgdbError SetAttributes(Row attributes);
  //@}

  /// @name Constructors and Destructors
  //@{
  /// The class constructor.
  Raster();

  /// The class destructor.
  ~Raster();
  //@}

private:

  Raster(const Raster&)             { }
  Raster& operator=(const Raster&)  { return *this; }
};

};  // namespace FileGDBAPI
