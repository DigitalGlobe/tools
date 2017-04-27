// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit tests for the EntityMapper class.

#include "kml/engine/entity_mapper.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/file.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmldom::DocumentPtr;
using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;

namespace kmlengine {

class EntityMapperTest : public testing::Test {
 protected:
  KmlFilePtr kml_file_;
};

// A hunk of KML that uses all possible entities.
const static string kEntityKml(
  "<Document>\n"
  "  <Schema name=\"TrailHeadType\" id=\"TrailHeadTypeId\">\n"
  "    <SimpleField type=\"string\" name=\"TrailHeadName\">\n"
  "      <displayName><![CDATA[<b>Trail Head Name</b>]]></displayName>\n"
  "    </SimpleField>\n"
  "    <SimpleField type=\"double\" name=\"TrailLength\">\n"
  "      <displayName><![CDATA[<i>The length in miles</i>]]></displayName>\n"
  "    </SimpleField>\n"
  "    <SimpleField type=\"int\" name=\"ElevationGain\">\n"
  "      <displayName><![CDATA[<i>change in altitude</i>]]></displayName>\n"
  "    </SimpleField>\n"
  "  </Schema>\n"
  "  <Placemark id=\"foo\" targetId=\"bar\">\n"
  "    <name>__NAME__</name>\n"
  "    <address>__ADDRESS__</address>\n"
  "    <Snippet>__SNIPPET__</Snippet>\n"
  "    <description>__DESCRIPTION__</description>\n"
  "    <ExtendedData>\n"
  "      <Data name=\"holeNumber\">\n"
  "        <value>1</value>\n"
  "      </Data>\n"
  "      <Data name=\"holeYardage\">\n"
  "        <value>234</value>\n"
  "      </Data>\n"
  "      <SchemaData schemaUrl=\"#TrailHeadTypeId\">\n"
  "        <SimpleData name=\"TrailHeadName\">Mount Everest</SimpleData>\n"
  "        <SimpleData name=\"TrailLength\">347.45</SimpleData>\n"
  "        <SimpleData name=\"ElevationGain\">10000</SimpleData>\n"
  "      </SchemaData>\n"
  "    </ExtendedData>\n"
  "  </Placemark>\n"
  "</Document>"
);

// A table mapping all entities contained in the above KML document with their
// expected replacement strings.
const static struct {
  string entity;
  string replacement;
} kEntityMap[] = {
  {"id", "foo"},
  {"targetId", "bar"},
  {"name", "__NAME__"},
  {"address", "__ADDRESS__"},
  {"Snippet", "__SNIPPET__"},
  {"description", "__DESCRIPTION__"},
  {"TrailHeadType/ElevationGain", "10000"},
  {"TrailHeadType/ElevationGain/displayName", "<i>change in altitude</i>"},
  {"TrailHeadType/TrailHeadName", "Mount Everest"},
  {"TrailHeadType/TrailHeadName/displayName", "<b>Trail Head Name</b>"},
  {"TrailHeadType/TrailLength", "347.45"},
  {"TrailHeadType/TrailLength/displayName", "<i>The length in miles</i>"},
  {"holeNumber", "1"},
  {"holeYardage", "234"},
};

TEST_F(EntityMapperTest, TestGetEntityFields) {
  string errs;
  kml_file_ = KmlFile::CreateFromParse(kEntityKml, NULL);
  ASSERT_TRUE(kml_file_);
  ASSERT_TRUE(errs.empty());

  DocumentPtr doc = kmldom::AsDocument(kml_file_->get_root());
  PlacemarkPtr p = kmldom::AsPlacemark(doc->get_feature_array_at(0));

  kmlbase::StringMap entity_map;
  EntityMapper entity_mapper(kml_file_, &entity_map);
  entity_mapper.GetEntityFields(p);

  // Verify that the correct number of entities were extracted from the KML.
  const size_t kSizeEntityMap = sizeof(kEntityMap)/sizeof(kEntityMap[0]);
  ASSERT_EQ(kSizeEntityMap, entity_map.size());

  // Verify that the entity map was populated as expected.
  for (size_t i = 0; i < kSizeEntityMap; ++i) {
    // The entity exists within the map.
    ASSERT_TRUE(entity_map.find(kEntityMap[i].entity) != entity_map.end());
    // The entity has the expected replacement text.
    ASSERT_EQ(kEntityMap[i].replacement,
                         entity_map[kEntityMap[i].entity]);
  }
}

// A table mapping entities to their expected replacements.
const static struct {
  string raw_text;
  string expanded_text;
} kReplacments[] = {
  {
    "abcdef",
    "abcdef",
  },
  {
    "abc$[]def",
    "abc$[]def",
  },
  {
    "abc$[noSuchEntity]def",
    "abc$[noSuchEntity]def",
  },
  {
    "$[name]$[description]",
    "__NAME____DESCRIPTION__",
  },
  {
    "xxx$[name]xxx$[description]xxx",
    "xxx__NAME__xxx__DESCRIPTION__xxx",
  },
  {
    "  $[name]  $[description]  ",
    "  __NAME__  __DESCRIPTION__  ",
  },
  {
    "  $[name]$[name]  $[description] $[description] $[name]",
    "  __NAME____NAME__  __DESCRIPTION__ __DESCRIPTION__ __NAME__",
  },
};

TEST_F(EntityMapperTest, TestCreateExpandedEntities) {
  kml_file_ = KmlFile::CreateFromParse(kEntityKml, NULL);
  DocumentPtr doc = kmldom::AsDocument(kml_file_->get_root());
  PlacemarkPtr p = kmldom::AsPlacemark(doc->get_feature_array_at(0));

  kmlbase::StringMap entity_map;
  EntityMapper entity_mapper(kml_file_, &entity_map);
  entity_mapper.GetEntityFields(p);

  // Verify that CreateExpandedEntities handles various kinds of entity
  // references, spacing, multiple references.
  for (size_t i = 0; i < sizeof(kReplacments)/sizeof(kReplacments[0]); ++i) {
    ASSERT_EQ(
        kReplacments[i].expanded_text,
        CreateExpandedEntities(kReplacments[i].raw_text, entity_map));
  }
}

TEST_F(EntityMapperTest, TestAltMarkupData) {
  const string kDataKml = (
    "<Placemark>"
    "<ExtendedData>"
    "<Data name=\"data1\">"
    "<displayName>1st display name</displayName>"
    "<value>1st</value>"
    "</Data>"
    "<Data name=\"data2\">"
    "<displayName>2nd display name</displayName>"
    "<value>2nd</value>"
    "</Data>"
    "<!-- The below are legal, but are in Google Earth they are overridden by"
    "the Feature's name and description for the balloon text replacements. -->"
    "<Data name=\"name\"><value>data name</value></Data>"
    "<Data name=\"description\"><value>data description</value></Data>"
    "</ExtendedData>"
    "</Placemark>"
  );
  const struct {
    const char* key;
    const char* value;
  } kKeyValues[] = {
    {
      "1st display name",
      "1st",
    },
    {
      "2nd display name",
      "2nd",
    },
    {
      "name",
      "data name",
    },
    {
      "description",
      "data description",
    },
  };

  kml_file_ = KmlFile::CreateFromParse(kDataKml, NULL);
  ASSERT_TRUE(kml_file_);
  PlacemarkPtr p = kmldom::AsPlacemark(kml_file_->get_root());
  ASSERT_TRUE(p);
  kmlbase::StringMap entity_map;
  kmlbase::StringPairVector alt_markup_map;
  EntityMapper entity_mapper(kml_file_, &entity_map, &alt_markup_map);
  entity_mapper.GetEntityFields(p);
  ASSERT_EQ(static_cast<size_t>(6), entity_map.size());
  ASSERT_EQ(static_cast<size_t>(4), alt_markup_map.size());
  for (int i = 0; i < 4; i++) {
    ASSERT_EQ(kKeyValues[i].key, alt_markup_map[i].first);
    ASSERT_EQ(kKeyValues[i].value, alt_markup_map[i].second);
  }
  // If GetEntityFields is called multiple times, the StringMap is overwritten
  // and the StringPairVector is appended to.
  entity_mapper.GetEntityFields(p);
  ASSERT_EQ(static_cast<size_t>(6), entity_map.size());
  ASSERT_EQ(static_cast<size_t>(8), alt_markup_map.size());
  entity_mapper.GetEntityFields(p);
  ASSERT_EQ(static_cast<size_t>(6), entity_map.size());
  ASSERT_EQ(static_cast<size_t>(12), alt_markup_map.size());
}

TEST_F(EntityMapperTest, TestAltMarkupSchemaData) {
  const struct {
    const char* key;
    const char* value;
  } kSchemaDataMappings[] = {
    {
      "s_name:simple field display name 1",
      "one",
    },
    {
      "s_name:sfield2",
      "2",
    },
  };

  const string kSchemaDataKml = string(DATADIR) + "/kml/schemadata.kml";
  string data;
  string errors;
  ASSERT_TRUE(File::ReadFileToString(kSchemaDataKml, &data));
  kml_file_ = KmlFile::CreateFromParse(data, &errors);
  ASSERT_FALSE(data.empty());
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(kml_file_);
  KmlPtr kml = kmldom::AsKml(kml_file_->get_root());
  DocumentPtr doc = kmldom::AsDocument(kml->get_feature());
  PlacemarkPtr p = kmldom::AsPlacemark(doc->get_feature_array_at(0));
  ASSERT_TRUE(p);
  kmlbase::StringMap entity_map;
  kmlbase::StringPairVector alt_markup_map;
  EntityMapper entity_mapper(kml_file_, &entity_map, &alt_markup_map);
  entity_mapper.GetEntityFields(p);
  ASSERT_EQ(static_cast<size_t>(2), alt_markup_map.size());
  for (int i = 0; i < 2; i++) {
    ASSERT_EQ(kSchemaDataMappings[i].key, alt_markup_map[i].first);
    ASSERT_EQ(kSchemaDataMappings[i].value, alt_markup_map[i].second);
  }
}

}  // end namespace kmlengine
