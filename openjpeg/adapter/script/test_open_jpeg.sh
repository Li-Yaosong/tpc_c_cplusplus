#!/bin/bash

# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

j2kRandomTileAccess="j2k_random_tile_access"
testDecodeArea="test_decode_area"
testTileEncoder="test_tile_encoder"
testTileDecoder="test_tile_decoder"
testempty0="testempty0"
testempty1="testempty1"
testempty2="testempty2"



function exe()
{
exe=$1
num=$#

	if [ $exe == $j2kRandomTileAccess ]
    then
		tag=$2
		echo "tag= $tag,exe = $exe"
		
		if [ -e $tag ]
		then 
			chmod +x $exe
			./$exe $tag
		else
			echo "$tag is not exist"
		fi
	elif [ $exe == $testDecodeArea ]
    then
		if [ $num == 3 ]
		then
			tag=$2
			flag=$1
			if [ -e $tag ]
			then
				./$exe $flag $tag
			fi
		elif [ $num == 6 ]
		then
			tag=$5
			if [ -e $tag ]
			then
			    echo "./$*"
				./$*
			fi
		else
			echo "exe $exe cmd failed!"
		fi
	elif [ $exe == $testTileEncoder ] 
	then
		if [ $num == 1 ]
		then
			./$exe
		else
			echo "./$*"
			./$*
		fi
	elif [ $exe == $testTileDecoder ]
	then
	    echo "$*"
		if [ $num == 1 ]
		then
			./$*
		elif [ $num == 5 ]
		then
			if [ -e $5 ]
			then 
				./$*
			fi
		fi
	else
		./$exe
	fi
}


function check_result()
{
exe="md5sum"
tag=$1
sum=$2
	if [ -e $tag ]
	then
        tmp=`md5sum $tag`
		result=${tmp%% *}
	
		if [ $result == $sum ]
	    then
			echo "test ok!"
		fi
	fi
}



function run_test()
{
    exe $testTileEncoder
	check_result test.j2k d50d345b669102ce18f7da78dc076546  
	exe $testTileEncoder 3 2048 2048 1024 1024 8 1 tte1.j2k
	check_result tte1.j2k dcb7dbfc7a3f552f5f01c35fb077a731  
	exe $testTileEncoder 3 2048 2048 1024 1024 8 1 tte2.jp2
	check_result tte2.jp2 433f8b10a513510f490651d51637b1c0  
	exe $testTileEncoder 1 2048 2048 1024 1024 8 1 tte3.j2k
	check_result tte3.j2k 92f6fa2693a01ea0e184a9cb79261935  
	exe $testTileEncoder 1  256  256  128  128 8 0 tte4.j2k
	check_result tte4.j2k ddbd0406e23e35d9d76d53eec0aba6f7  
	exe $testTileEncoder 1  512  512  256  256 8 0 tte5.j2k
	check_result tte5.j2k f41e019f8a879220769b3122723448f1  
	exe $testTileDecoder
	check_result test.j2k d50d345b669102ce18f7da78dc076546  
	exe $testTileDecoder 0 0 1024 1024 tte1.j2k
	check_result tte1.j2k dcb7dbfc7a3f552f5f01c35fb077a731  
	exe $testTileDecoder 0 0 1024 1024 tte2.jp2
	check_result tte2.jp2  433f8b10a513510f490651d51637b1c0  
	exe $j2kRandomTileAccess tte1.j2k
	exe $j2kRandomTileAccess tte2.jp2
	exe $j2kRandomTileAccess tte3.j2k
	exe $j2kRandomTileAccess tte4.j2k
	exe $j2kRandomTileAccess tte5.j2k
	exe $testTileEncoder 1 256 256 32 32 8 0 reversible_no_precinct.j2k 4 4 3 0 0 0
	check_result reversible_no_precinct.j2k 5a03aed80730bb312b93a373af2619a7  
	exe $testDecodeArea -q reversible_no_precinct.j2k
	check_result reversible_no_precinct.j2k 5a03aed80730bb312b93a373af2619a7 
	exe $testTileEncoder 1 203 201 17 19 8 0 reversible_203_201_17_19_no_precinct.j2k 4 4 3 0 0 0
	check_result reversible_203_201_17_19_no_precinct.j2k 080fe377819420d1bbac8c2a209c3faf  
	exe $testDecodeArea -q reversible_203_201_17_19_no_precinct.j2k
	check_result reversible_203_201_17_19_no_precinct.j2k 080fe377819420d1bbac8c2a209c3faf
	exe $testTileEncoder 1 256 256 32 32 8 0 reversible_with_precinct.j2k 4 4 3 0 0 0 16 16
	check_result reversible_with_precinct.j2k 2d7074173d443ad9e348b9f4db955994  
	exe $testDecodeArea -q reversible_with_precinct.j2k
	check_result reversible_with_precinct.j2k 2d7074173d443ad9e348b9f4db955994
	exe $testTileEncoder 1 256 256 32 32 8 1 irreversible_no_precinct.j2k 4 4 3 0 0 0
	check_result irreversible_no_precinct.j2k 9e8c1ac0e7c920e559a8deb74cc2e094  
	exe $testDecodeArea -q irreversible_no_precinct.j2k
	check_result irreversible_no_precinct.j2k 9e8c1ac0e7c920e559a8deb74cc2e094
	exe $testTileEncoder 1 203 201 17 19 8 1 irreversible_203_201_17_19_no_precinct.j2k 4 4 3 0 0 0
	check_result irreversible_203_201_17_19_no_precinct.j2k d58da7bdddbbeb733474f30757a31a87  
	exe $testDecodeArea -q irreversible_203_201_17_19_no_precinct.j2k
	heck_result irreversible_203_201_17_19_no_precinct.j2k d58da7bdddbbeb733474f30757a31a87
	exe $testTileEncoder 1 256 256 256 256 8 0 tda_single_tile.j2k
	check_result tda_single_tile.j2k 35737f347486a97cbdd9a545df382584
	exe $testDecodeArea -q -strip_height 3 -strip_check tda_single_tile.j2k
	check_result tda_single_tile.j2k 35737f347486a97cbdd9a545df382584
	exe $testempty0
	exe $testempty1
	exe $testempty2
}

run_test

