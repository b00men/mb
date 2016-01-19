#!/usr/bin/env bash
source ./lib_install.sh

check_permission
set_var
install_depending
build_and_install
config_mysql
config_nginx
config_mb
config_mb_daemon
run_and_epilogue