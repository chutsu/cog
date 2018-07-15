#!/bin/sh
COMMAND='valgrind --error-exitcode=-1 --leak-check=full'
TESTS_DIR='bin'
TEST_FORMAT="*_tests"
LOG_TESTS=0
LOG_PATH=$TESTS_DIR/log
TMP_OUTPUT=/tmp/output.tmp
FAILED=0
ALL_PASSED=1

GREEN="\033[32m"
RED="\033[31m"
ENDTC="\033[0m"


strip_term_colors()
{
  SED_ARGS="s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g"
  SED_FLAGS=-r

  cat $LOG_FILE | sed $SED_FLAGS $SED_ARGS > $LOG_FILE.processed
  mv $LOG_FILE.processed $LOG_FILE
}

print_passed()
{
  echo  "${GREEN}PASSED!${ENDTC}"
}

print_failed()
{
  echo "${RED}FAILED!${ENDTC}"
  echo "--------------------------------------------------"
  cat $TMP_OUTPUT
  echo "--------------------------------------------------"
}

check_mem_leaks()
{
  VALGRIND_MODE=$(echo $COMMAND | grep "valgrind" | echo $?)

  if [ "$VALGRIND_MODE" -eq "0" ]
  then

    RES=$(cat $TMP_OUTPUT | grep "no leaks are possible")

    if [ "$RES" = "" ]
    then
      FAILED=1
      ALL_PASSED=0
    fi
  fi
}

check_exit_status()
{
  FAILED=$?
}

analyze_test_results()
{
  check_exit_status
  check_mem_leaks

  if [ $FAILED -eq 1 ]
  then
    print_failed
  else
    print_passed
  fi
}

log_test()
{
  mkdir -p $LOG_PATH
  LOG_FILE=$LOG_PATH/$(basename ${TEST}.log)
  cp $TMP_OUTPUT $LOG_FILE
}

run_test()
{
  # run test
  printf "TEST [$TEST] "
  $COMMAND $TEST > $TMP_OUTPUT 2>&1
  analyze_test_results

  # log test
  if [ $LOG_TESTS -eq 1 ]
  then
    log_test
    strip_term_colors
  fi
}

main()
{
  # run tests
  TESTS=$(find ${TESTS_DIR} -type f -name ${TEST_FORMAT})
  for TEST in $TESTS
  do
    run_test
  done

  # exit properly
  if [ $ALL_PASSED -eq 0 ]
  then
    exit 1;
  else
    exit 0;
  fi
}


# RUN
main
