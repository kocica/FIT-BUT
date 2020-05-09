#!/usr/bin/env bash

# REASON: Creates 4 empty files for tests
# USAGE: ./createTest <test_number> -> ./createTest 51

TEST_NUMBER="$1"

touch "test0${TEST_NUMBER}.in"
touch "test0${TEST_NUMBER}_i.out"
touch "test0${TEST_NUMBER}_1.out"
touch "test0${TEST_NUMBER}_2.out"