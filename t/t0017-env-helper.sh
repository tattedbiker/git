#!/bin/sh

test_description='test env--helper'

. ./test-lib.sh


test_expect_success 'env--helper usage' '
	test_must_fail git env--helper &&
	test_must_fail git env--helper --mode-bool &&
	test_must_fail git env--helper --mode-ulong &&
	test_must_fail git env--helper --mode-bool --variable &&
	test_must_fail git env--helper --mode-bool --variable --default &&
	test_must_fail git env--helper --mode-bool --variable= --default=
'

test_expect_success 'env--helper bad default values' '
	test_must_fail git env--helper --mode-bool --variable=MISSING --default=1xyz &&
	test_must_fail git env--helper --mode-ulong --variable=MISSING --default=1xyz
'

test_expect_success 'env--helper --mode-bool' '
	echo 1 >expected &&
	git env--helper --mode-bool --variable=MISSING --default=1 --exit-code >actual &&
	test_cmp expected actual &&

	echo 0 >expected &&
	test_must_fail git env--helper --mode-bool --variable=MISSING --default=0 --exit-code >actual &&
	test_cmp expected actual &&

	git env--helper --mode-bool --variable=MISSING --default=0 >actual &&
	test_cmp expected actual &&

	>expected &&
	git env--helper --mode-bool --variable=MISSING --default=1 --exit-code --quiet >actual &&
	test_cmp expected actual &&

	EXISTS=true git env--helper --mode-bool --variable=EXISTS --default=0 --exit-code --quiet >actual &&
	test_cmp expected actual &&

	echo 1 >expected &&
	EXISTS=true git env--helper --mode-bool --variable=EXISTS --default=0 --exit-code >actual &&
	test_cmp expected actual
'

test_expect_success 'env--helper --mode-ulong' '
	echo 1234567890 >expected &&
	git env--helper --mode-ulong --variable=MISSING --default=1234567890 --exit-code >actual &&
	test_cmp expected actual &&

	echo 0 >expected &&
	test_must_fail git env--helper --mode-ulong --variable=MISSING --default=0 --exit-code >actual &&
	test_cmp expected actual &&

	git env--helper --mode-ulong --variable=MISSING --default=0 >actual &&
	test_cmp expected actual &&

	>expected &&
	git env--helper --mode-ulong --variable=MISSING --default=1234567890 --exit-code --quiet >actual &&
	test_cmp expected actual &&

	EXISTS=1234567890 git env--helper --mode-ulong --variable=EXISTS --default=0 --exit-code --quiet >actual &&
	test_cmp expected actual &&

	echo 1234567890 >expected &&
	EXISTS=1234567890 git env--helper --mode-ulong --variable=EXISTS --default=0 --exit-code >actual &&
	test_cmp expected actual
'

test_done
