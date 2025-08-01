//
// GeodatabaseManagement.h
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

/// A set of functions for accessing, creating and deleting file geodatabases.
/// @file GeodatabaseManagement.h

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

class Geodatabase;

/// Creates a new 10.x file geodatabase in the specified location.
/// If the file geodatabase already exists a -2147220653 (The table already exists) error will be returned.
/// If the path is seriously in error, say pointing to the wrong drive, a -2147467259 (E_FAIL) error is returned.
/// @param[in]    path The location where the geodatabase should be created.
/// @param[out]   geodatabase A reference to the newly-created geodatabase.
/// @return       Error code indicating whether the method finished successfully.
EXT_FILEGDB_API fgdbError CreateGeodatabase(const std::wstring& path, Geodatabase& geodatabase);

/// Opens an existing 10.x file geodatabase.
/// If the path is incorrect a -2147024894 (The system cannot find the file specified) error will be returned. If the
/// release is pre-10.x a -2147220965 (This release of the GeoDatabase is either invalid or out of date) error will be returned.
/// @param[in]    path The path of the geodatabase.
/// @param[out]   geodatabase A reference to the opened geodatabase.
/// @return       Error code indicating whether the method finished successfully.
EXT_FILEGDB_API fgdbError OpenGeodatabase(const std::wstring& path, Geodatabase& geodatabase);

/// Closes an open file geodatabase.
/// @param[in]    geodatabase A reference to the geodatabase.
/// @return       Error code indicating whether the method finished successfully.
EXT_FILEGDB_API fgdbError CloseGeodatabase(Geodatabase& geodatabase);

/// Deletes a file geodatabase.
/// If the path is incorrect a -2147024894 (The system cannot find the file specified) error will be returned.
/// If another process has a lock on the geodatabase, a -2147220947 (Cannot acquire a lock) error will be returned.
/// If access is denied an E_FAIL is returned.
/// @param[in]    path The path of the geodatabase.
/// @return       Error code indicating whether the method finished successfully.
EXT_FILEGDB_API fgdbError DeleteGeodatabase(const std::wstring& path);

};  // namespace FileGDBAPI
