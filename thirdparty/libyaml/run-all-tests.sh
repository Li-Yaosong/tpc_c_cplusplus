#!/bin/sh
ctest

total_tests=13
passed_tests=2
cat ../examples/anchors.yaml | ./example-deconstructor
res=$?
if [ $res -ne 0 ]
then
  echo "3/13 example-deconstructor .......................... failed"
else
  echo "3/13 example-deconstructor .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

cat ../examples/anchors.yaml | ./example-deconstructor-alt
res=$?
if [ $res -ne 0 ]
then
  echo "4/13 example-deconstructor-alt .......................... failed"
else
  echo "4/13 example-deconstructor-alt .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

cat ../examples/anchors.yaml | ./example-reformatter
res=$?
if [ $res -ne 0 ]
then
  echo "5/13 example-reformatter .......................... failed"
else
  echo "5/13 example-reformatter .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

cat ../examples/anchors.yaml | ./example-reformatter-alt
res=$?
if [ $res -ne 0 ]
then
  echo "6/13 example-reformatter-alt .......................... failed"
else
  echo "6/13 example-reformatter-alt .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-dumper ../examples/global-tag.yaml 
res=$?
if [ $res -ne 0 ]
then
  echo "7/13 run-dumper .......................... failed"
else
  echo "7/13 run-dumper .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-emitter ../examples/global-tag.yaml 
res=$?
if [ $res -ne 0 ]
then
  echo "8/13 run-emitter .......................... failed"
else
  echo "8/13 run-emitter .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-parser-test-suite ../examples/anchors.yaml | ./run-emitter-test-suite
res=$?
if [ $res -ne 0 ]
then
  echo "9/13 run-emitter-test-suite .......................... failed"
else
  echo "9/13 run-emitter-test-suite .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-loader ../examples/global-tag.yaml 
res=$?
if [ $res -ne 0 ]
then
  echo "10/13 run-loader .......................... failed"
else
  echo "10/13 run-loader .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-parser ../examples/global-tag.yaml 
res=$?
if [ $res -ne 0 ]
then
  echo "11/13 run-parser .......................... failed"
else
  echo "11/13 run-parser .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-parser-test-suite ../examples/anchors.yaml 
res=$?
if [ $res -ne 0 ]
then
  echo "12/13 run-parser-test-suite .......................... failed"
else
  echo "12/13 run-parser-test-suite .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

./run-scanner ../examples/global-tag.yaml
res=$?
if [ $res -ne 0 ]
then
  echo "13/13 run-scanner .......................... failed"
else
  echo "13/13 run-scanner .......................... passed"
  passed_tests=`expr $passed_tests + 1`
fi

test_fail=$((total_tests - passed_tests)) 
if [ $total_tests -ne 13 ]
then
  echo "$total_tests tests passed, $test_fail tests failed"
else
  echo "100% tests passed, 0 tests failed out of 13"
fi
