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

@test "remote test for common command" {
    ./dsh -s -p 12345 &
    server_pid=$!
    sleep 1

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
echo derrik
EOF

    kill $server_pid
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:12345dsh4>derrikdsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "remote test for exit command" {
    ./dsh -s -p 12345 &
    server_pid=$!
    sleep 1

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
exit
EOF

    kill $server_pid
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:12345dsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "remote test for pipeline command" {
    ./dsh -s -p 12350 &
    server_pid=$!
    sleep 1

    run ./dsh -c -i 127.0.0.1 -p 12350 <<EOF
echo "HelloWorld" | grep "World"
EOF

    kill $server_pid
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:12350dsh4>HelloWorlddsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "remote test for invalid command" {
    ./dsh -s -p 12345 &
    server_pid=$!
    sleep 1

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
badcommand
EOF

    kill $server_pid
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:12345dsh4>execvp:Permissiondeniedsocketservermode:addr:0.0.0.0:12345->Single-ThreadedModedsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
