#!/bin/bash

gcc lab.c -o lab

commands=(MakeDirectory ShowDirectory RemoveDirectory CreateFile PrintFile RemoveFile CreateSymbolLink PrintSymbolLinkSelf PrintSymbolLinkFile CreateHardLink RemoveHardLink PrintAccess ChangeAccess)

for cmd in "${commands[@]}"; do
    ln "lab" "$cmd"
done
