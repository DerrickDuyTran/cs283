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

@test "provided command test" {
    run ./dsh <<EOF
ls | grep ".c"
EOF
    expected_output="dsh_cli.c"
    expected_output_2="dshlib.c"

    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
    [[ "$output" == *"$expected_output_2"* ]]
}


@test "number of c files test" {
    run ./dsh <<EOF
ls | grep ".c" | wc -l
EOF
    expected_output="2"
    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "exit pipe test" {
    run ./dsh <<EOF
echo "test" | exit
EOF
    [ "$status" -eq 0 ]
}

@test "empty pipe test" {
    run ./dsh <<EOF
|
EOF
    expected_output="warning: no commands provided"

    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "check max pipes test " {
    run ./dsh <<EOF
echo "x" | echo "x" | echo "x" | echo "x" | echo "x" | echo "x" | echo "x" | echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"| echo "x"
EOF
    expected_output="error: piping limited to 8 commands"
    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

