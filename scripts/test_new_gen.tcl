#!/usr/bin/tclsh
#
#

package require fileutil


set ccngen_dir [file normalize [lindex $argv 0]]
set coconut_dir [file normalize [lindex $argv 1]]

set tmp [ fileutil::tempdir ]
set tmp_coconut $tmp/coconut-test/

file delete -force $tmp_coconut
file copy $coconut_dir $tmp/coconut-test
file delete -force $tmp_coconut/cocogen/ccngen
file copy $ccngen_dir $tmp_coconut/cocogen
file delete -force $tmp_coconut/build
puts "===================================RUNNING==============================="

if {[ catch {
	exec make -C $tmp_coconut
   } err ]} { 
	puts $err
	puts "===================================FAILED==============================="
	exit 1
} 
cd $tmp_coconut/build
exec ./cocogen/cocogen $coconut_dir/cocogen/main.ccn
file delete -force $tmp_coconut/cocogen/ccngen
file copy $tmp_coconut/build/ccngen $tmp_coconut/cocogen

if {[ catch {
	exec make -C $tmp_coconut
	cd $tmp_coconut/build
	exec ctest -R "GoodDSLfiles"
   } err ]} { 
	puts $err
	puts "===================================FAILED==============================="
	exit 1
} else {
	puts "===================================SUCCESS==============================="
}
