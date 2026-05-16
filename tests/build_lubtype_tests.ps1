# build_lubtype_tests.ps1
# PowerShell build script for lubtype_tests.exe
# Usage: Run from the tests directory or invoke with a full path
# Copyright (c) 2026 paulsinclair51  
# SPDX-License-Identifier: MIT  
# For license details, see the LICENSE file in the project root.

function Find-VcVars64 {
    $vswhere = "$env:ProgramFiles(x86)\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
        if ($vsPath) {
            $vcvars = Join-Path $vsPath 'VC\Auxiliary\Build\vcvars64.bat'
            if (Test-Path $vcvars) { return $vcvars }
        }
    }
    $fallbacks = @(
        'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat',
        'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat'
    )
    foreach ($path in $fallbacks) { if (Test-Path $path) { return $path } }
    return $null
}

Push-Location $PSScriptRoot

try {

function Invoke-Checked {
    param([string]$Cmd)
    Write-Host "> $Cmd"
    Invoke-Expression $Cmd
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

$sourceFiles = @(
    'lubtype_tests.c',
    'test_error_edge.c',
    'test_advanced_ops.c',
    'test_cmp_search.c',
    'test_reserved_matrix.c',
    'test_search_families.c',
    'test_core_families.c',
    'test_type_matrix.c',
    'test_utilities.c',
    'test_fuzz_edge.c',
    'test_skip.c'
)

if ($IsWindows) {
    # Check if cl.exe is available
    $cl = Get-Command cl.exe -ErrorAction SilentlyContinue
    if (-not $cl) {
        Write-Host '[INFO] MSVC environment not detected. Attempting to load vcvars64.bat...'
        $vcvars = Find-VcVars64
        if ($vcvars) {
            cmd /c "`"$vcvars`" && set" | ForEach-Object {
                if ($_ -match '^(\w+)=(.*)$') {
                    [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
                }
            }
        } else {
            Write-Error '[ERROR] Could not find vcvars64.bat. Please run this script from a Developer Command Prompt.'
            exit 1
        }
    }

    # Build object files first, then link one executable.
    $baseFlags = '/nologo /W4 /EHsc /I.. /D_CRT_SECURE_NO_WARNINGS'
    $objectFiles = @()

    foreach ($src in $sourceFiles) {
        $obj = [System.IO.Path]::GetFileNameWithoutExtension($src) + '.obj'
        Invoke-Checked "cl $baseFlags /c $src /Fo:$obj"
        $objectFiles += $obj
    }

    Invoke-Checked "cl $baseFlags /c ..\lubdefinitions.c /Fo:lubdefinitions.obj"
    $objectFiles += 'lubdefinitions.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_L /c test_charclass.c /Fo:test_charclass_l.obj"
    $objectFiles += 'test_charclass_l.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_U /c test_charclass.c /Fo:test_charclass_u.obj"
    $objectFiles += 'test_charclass_u.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_L /c test_strlen_strclass.c /Fo:test_strlen_strclass_l.obj"
    $objectFiles += 'test_strlen_strclass_l.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_L /c test_count.c /Fo:test_count_l.obj"
    $objectFiles += 'test_count_l.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_U /c test_count.c /Fo:test_count_u.obj"
    $objectFiles += 'test_count_u.obj'

    Invoke-Checked "cl $baseFlags /DLUB_X_IS_U /c test_strlen_strclass.c /Fo:test_strlen_strclass_u.obj"
    $objectFiles += 'test_strlen_strclass_u.obj'

    Invoke-Checked "cl /nologo /Fe:lubtype_tests.exe $($objectFiles -join ' ')"

    # Cleanup intermediate object files after successful link.
    Remove-Item -Force -ErrorAction SilentlyContinue *.obj
    Write-Host 'Build complete: lubtype_tests.exe (intermediate .obj files removed)'
}
else {
    # Linux/macOS fallback using cc/clang/gcc.
    $cc = Get-Command cc -ErrorAction SilentlyContinue
    if (-not $cc) {
        $cc = Get-Command clang -ErrorAction SilentlyContinue
    }
    if (-not $cc) {
        $cc = Get-Command gcc -ErrorAction SilentlyContinue
    }
    if (-not $cc) {
        Write-Error '[ERROR] Could not find cc, clang, or gcc in PATH.'
        exit 1
    }

    $ccExe = $cc.Source
    $baseFlags = '-I.. -std=c11 -Wall -Wextra -O2'
    $objectFiles = @()

    foreach ($src in $sourceFiles) {
        $obj = [System.IO.Path]::GetFileNameWithoutExtension($src) + '.o'
        Invoke-Checked "$ccExe $baseFlags -c $src -o $obj"
        $objectFiles += $obj
    }

    Invoke-Checked "$ccExe $baseFlags -c ../lubdefinitions.c -o lubdefinitions.o"
    $objectFiles += 'lubdefinitions.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_L -c test_charclass.c -o test_charclass_l.o"
    $objectFiles += 'test_charclass_l.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_U -c test_charclass.c -o test_charclass_u.o"
    $objectFiles += 'test_charclass_u.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_L -c test_strlen_strclass.c -o test_strlen_strclass_l.o"
    $objectFiles += 'test_strlen_strclass_l.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_L -c test_count.c -o test_count_l.o"
    $objectFiles += 'test_count_l.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_U -c test_count.c -o test_count_u.o"
    $objectFiles += 'test_count_u.o'

    Invoke-Checked "$ccExe $baseFlags -DLUB_X_IS_U -c test_strlen_strclass.c -o test_strlen_strclass_u.o"
    $objectFiles += 'test_strlen_strclass_u.o'

    Invoke-Checked "$ccExe $baseFlags -o lubtype_tests $($objectFiles -join ' ')"

    # Cleanup intermediate object files after successful link.
    Remove-Item -Force -ErrorAction SilentlyContinue *.o
    Write-Host 'Build complete: lubtype_tests (intermediate .o files removed)'
}
}
finally {
    Pop-Location
}
