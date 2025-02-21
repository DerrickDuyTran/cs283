#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Invalid directory for cd command" {
    run "./dsh" <<EOF
cd /nonexistentdirectory
exit
EOF
    echo "$output"
    echo "$status"
    echo "$output" | grep -q "chdir:"
    [ "$status" -eq 0 ]
}

@test "Non-existent command error handling" {
    run "./dsh" <<EOF
nonexistentcmd
exit
EOF
    echo "$output"
    echo "$status"
    echo "$output" | grep -q "execvp:"
    [ "$status" -eq 0 ]
}

@test "cd with valid directory" {
    tmpdir="/tmp/dsh-test-$$"
    mkdir -p "$tmpdir"
    run "./dsh" <<EOF
pwd
cd $tmpdir
pwd
exit
EOF
    final_pwd=$(echo "$output" | grep -E "^/" | tail -n1)
    echo "$tmpdir"
    echo "$final_pwd"
    [ "$final_pwd" = "$tmpdir" ]
    [ "$status" -eq 0 ]
}

@test "cd with no arguments doesn't change directory" {
    run "./dsh" <<EOF
pwd
cd
pwd
exit
EOF
    first_pwd=$(echo "$output" | grep -E "^/" | head -n1)
    second_pwd=$(echo "$output" | grep -E "^/" | tail -n1)
    echo "$first_pwd"
    echo "$second_pwd"
    [ "$first_pwd" = "$second_pwd" ]
    [ "$status" -eq 0 ]
}