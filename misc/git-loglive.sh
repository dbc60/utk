#!/bin/bash

display_git_log() {
    clear
    git --no-pager log --graph --pretty=oneline --abbrev-commit --decorate --all $*
}

display_git_log $*
while sleep 2
do
    display_git_log $*
done

