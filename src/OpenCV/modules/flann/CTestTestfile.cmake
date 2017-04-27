# CMake generated Testfile for 
# Source directory: C:/tools/src/OpenCV/modules/flann
# Build directory: C:/tools/src/OpenCV/modules/flann
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(opencv_test_flann "C:/tools/src/OpenCV/bin/Debug/opencv_test_flannd.exe" "--gtest_output=xml:opencv_test_flann.xml")
  set_tests_properties(opencv_test_flann PROPERTIES  LABELS "Main;opencv_flann;Accuracy" WORKING_DIRECTORY "C:/tools/src/OpenCV/test-reports/accuracy")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(opencv_test_flann "C:/tools/src/OpenCV/bin/Release/opencv_test_flann.exe" "--gtest_output=xml:opencv_test_flann.xml")
  set_tests_properties(opencv_test_flann PROPERTIES  LABELS "Main;opencv_flann;Accuracy" WORKING_DIRECTORY "C:/tools/src/OpenCV/test-reports/accuracy")
else()
  add_test(opencv_test_flann NOT_AVAILABLE)
endif()
