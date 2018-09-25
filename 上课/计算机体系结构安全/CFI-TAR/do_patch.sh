#!/bin/bash
echo "patch *.patch to qemu"

patch -p1 < main_modify.patch

patch -p1 < exec-all_modify.patch

patch -p1 < translate_modify.patch

patch -p1 < cpu-exec_modify.patch

echo "patch finish"


