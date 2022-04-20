#!/usr/bin/tclsh
#
#

package require fileutil

set coconut_dir [file normalize [lindex $argv 0]]
set target [lindex $argv 1]


proc cleanup {tmp} {
	puts "Cleaning up"
	file delete -force $tmp
}

proc build {tmp} {
	puts "===================================BUILDING==============================="
	if {[ catch {
		exec make -C $tmp/coconut/build
	} err ]} {
		puts $err
		puts "===================================FAILED==============================="
		cleanup $tmp
		exit 1
	}
	puts "===================================SUCCESS==============================="
}

proc setup {coconut target} {
	set tmp [ fileutil::maketempdir -prefix coconut ]
	file copy $coconut $tmp
	file delete -force $tmp/coconut/build
	exec make -C $tmp/coconut $target
	return $tmp
}

proc gen_and_replace {tmp} {
	cd $tmp/coconut/build
	exec ./cocogen/cocogen $tmp/coconut/cocogen/main.ccn
	file delete -force $tmp/coconut/cocogen/ccngen
	file copy $tmp/coconut/build/ccngen $tmp/coconut/cocogen
}

proc run_tests {tmp} {
	if {[ catch {
		cd $tmp/coconut/build
		exec ctest -V
	} err ]} { 
		puts $err
		puts "===================================FAILED==============================="
		cleanup $tmp
		exit 1
	}

	puts "===================================SUCCESS==============================="
}

set tmp [setup $coconut_dir $target]
for {set i 0} {$i < 4} {incr i} {
	build $tmp
	gen_and_replace $tmp
}
run_tests $tmp
cleanup $tmp
exit 0


