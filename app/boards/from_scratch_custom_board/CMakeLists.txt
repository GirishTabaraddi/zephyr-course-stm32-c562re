cmake_minimum_required(VERSION 3.20.0)

# Declare this directory as a Zephyr library module
zephyr_library()

# Add your board.c file to the board's compilation pool
zephyr_library_sources(board.c)
