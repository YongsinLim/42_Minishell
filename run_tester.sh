#!/bin/bash

# Build the project
make re

# Clean all .o file
make clean

# Copy the executable to the tester directory
cp minishell ~/Desktop/42KL/tester/42_minishell_tester/

# Change to the tester directory
cd ~/Desktop/42KL/tester/42_minishell_tester

# Run the tester script
./tester.sh m
