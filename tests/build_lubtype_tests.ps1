# build_lubtype_tests.ps1
# PowerShell build script for lubtype_tests.exe
# Usage: Run from the Ext/tests directory or with full path
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

# Build command
$cmd = 'cl /nologo /W4 /EHsc /Fe:lubtype_tests.exe lubtype_tests.c test_*.c /I.. /D_CRT_SECURE_NO_WARNINGS'
Write-Host "> $cmd"
Invoke-Expression $cmd
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host 'Build complete: lubtype_tests.exe'
