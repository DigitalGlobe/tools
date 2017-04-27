#---
# File ossimjni/CMakeLists.txt
# $Id: CMakeLists.txt 19748 2011-06-12 15:34:56Z dburken $
#---

message("**************** begin ossimjni project cmake setup ****************")

cmake_minimum_required(VERSION 2.8)

project(ossimjni)

include(OssimVersion)
include(OssimUtilities)
include(OssimCommonVariables)

subdirs(cpp)

OSSIM_ADD_COMMON_MAKE_UNINSTALL()

message("**************** end ossimjni project cmake setup ****************")
