[CmdletBinding()]
param(
    [ValidateSet("C", "C++", "Rust", "Go")]
    [string]$Language,

    [ValidateSet("Overview", "Commands", "Practice", "Quiz", "Run")]
    [string]$Action
)

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$isWindowsHost = $env:OS -eq "Windows_NT"
$exeSuffix = if ($isWindowsHost) { ".exe" } else { "" }

$catalog = @{
    "C" = @{
        CheatSheet = "C.md"
        Example    = "examples/c/main.c"
        Topics     = @(
            "Pointers, arrays, structs, and manual memory management",
            "Preprocessor directives, file I/O, and function design",
            "Portable low-level programming with the standard library"
        )
        Practice   = @(
            "Write swap(int* a, int* b) and explain why pointers are needed.",
            "Read numbers from a file and compute min/max without overflowing buffers.",
            "Build a linked list and free every node safely."
        )
        Quiz       = @(
            @{ Prompt = "What header provides malloc and free?"; Answers = @("stdlib.h", "<stdlib.h>") }
            @{ Prompt = "Which operator dereferences a pointer?"; Answers = @("*") }
            @{ Prompt = "Which compiler flag enables the C11 standard with gcc?"; Answers = @("-std=c11") }
        )
    }
    "C++" = @{
        CheatSheet = "ModernCPP.md"
        Example    = "examples/cpp/main.cpp"
        Topics     = @(
            "RAII, smart pointers, and safer resource management",
            "STL containers, algorithms, lambdas, and concurrency",
            "Templates, variants, optionals, and modern application structure"
        )
        Practice   = @(
            "Replace a raw owning pointer with std::unique_ptr and describe the lifetime change.",
            "Use std::vector plus std::ranges or algorithms to transform and filter values.",
            "Model a small service with class methods, exceptions, and unit-style assertions."
        )
        Quiz       = @(
            @{ Prompt = "Which smart pointer has exclusive ownership?"; Answers = @("unique_ptr", "std::unique_ptr") }
            @{ Prompt = "Which header contains std::vector?"; Answers = @("vector", "<vector>") }
            @{ Prompt = "Which compiler flag enables the C++20 standard with g++?"; Answers = @("-std=c++20") }
        )
    }
    "Rust" = @{
        CheatSheet = "Rust.md"
        Example    = "examples/rust/main.rs"
        Topics     = @(
            "Ownership, borrowing, lifetimes, and pattern matching",
            "Enums, traits, Result handling, and iterator-heavy code",
            "Systems programming with strong safety guarantees"
        )
        Practice   = @(
            "Write a function that borrows a slice and returns the largest value safely.",
            "Convert nested conditionals into a match over an enum.",
            "Read a file into a String and propagate errors with Result."
        )
        Quiz       = @(
            @{ Prompt = "Which keyword declares an immutable variable in Rust?"; Answers = @("let") }
            @{ Prompt = "What enum is commonly used for fallible operations?"; Answers = @("result", "std::result", "std::result::result", "Result") }
            @{ Prompt = "Which command-line compiler builds a single Rust file?"; Answers = @("rustc") }
        )
    }
    "Go" = @{
        CheatSheet = "GO.md"
        Example    = "examples/go/main.go"
        Topics     = @(
            "Structs, interfaces, slices, maps, and explicit error handling",
            "Goroutines, channels, and simple concurrent workflows",
            "Practical tooling with gofmt, go run, and the standard library"
        )
        Practice   = @(
            "Create a worker goroutine that squares numbers from a channel.",
            "Parse CLI flags and validate required input before running logic.",
            "Implement a small interface and swap concrete types behind it."
        )
        Quiz       = @(
            @{ Prompt = "Which keyword starts a goroutine?"; Answers = @("go") }
            @{ Prompt = "What builtin type is used for concurrent communication?"; Answers = @("channel", "chan") }
            @{ Prompt = "Which command runs a Go file directly?"; Answers = @("go run") }
        )
    }
}

function Get-LanguageInfo {
    param([string]$Name)

    if (-not $catalog.ContainsKey($Name)) {
        throw "Unsupported language: $Name"
    }

    $entry = $catalog[$Name]
    $cheatSheetPath = Join-Path $repoRoot $entry.CheatSheet
    $examplePath = Join-Path $repoRoot $entry.Example
    $runSpec = Get-RunSpec -Name $Name -ExamplePath $examplePath

    return @{
        Name       = $Name
        CheatSheet = $cheatSheetPath
        Example    = $examplePath
        Topics     = $entry.Topics
        Practice   = $entry.Practice
        Quiz       = $entry.Quiz
        Command    = $runSpec.Command
        Tool       = $runSpec.Tool
    }
}

function Get-RunSpec {
    param(
        [string]$Name,
        [string]$ExamplePath
    )

    switch ($Name) {
        "C" {
            $outputPath = Join-Path ([System.IO.Path]::GetTempPath()) ("c-learning" + $exeSuffix)
            return @{
                Tool       = "gcc"
                OutputPath = $outputPath
                BuildArgs  = @("-std=c11", "-Wall", "-Wextra", "-pedantic", $ExamplePath, "-o", $outputPath)
                Command    = "gcc -std=c11 -Wall -Wextra -pedantic `"$ExamplePath`" -o `"$outputPath`" && `"$outputPath`""
            }
        }
        "C++" {
            $outputPath = Join-Path ([System.IO.Path]::GetTempPath()) ("cpp-learning" + $exeSuffix)
            return @{
                Tool       = "g++"
                OutputPath = $outputPath
                BuildArgs  = @("-std=c++20", "-Wall", "-Wextra", "-pedantic", $ExamplePath, "-o", $outputPath)
                Command    = "g++ -std=c++20 -Wall -Wextra -pedantic `"$ExamplePath`" -o `"$outputPath`" && `"$outputPath`""
            }
        }
        "Rust" {
            $outputPath = Join-Path ([System.IO.Path]::GetTempPath()) ("rust-learning" + $exeSuffix)
            return @{
                Tool       = "rustc"
                OutputPath = $outputPath
                BuildArgs  = @($ExamplePath, "-o", $outputPath)
                Command    = "rustc `"$ExamplePath`" -o `"$outputPath`" && `"$outputPath`""
            }
        }
        "Go" {
            return @{
                Tool       = "go"
                OutputPath = $null
                BuildArgs  = @("run", $ExamplePath)
                Command    = "go run `"$ExamplePath`""
            }
        }
    }

    throw "Unsupported language: $Name"
}

function Show-Overview {
    param([hashtable]$Info)

    Write-Host ""
    Write-Host "== $($Info.Name) overview ==" -ForegroundColor Cyan
    Write-Host "Cheat sheet: $($Info.CheatSheet)"
    Write-Host "Runnable example: $($Info.Example)"
    Write-Host "Focus areas:"
    foreach ($topic in $Info.Topics) {
        Write-Host "  - $topic"
    }
}

function Show-Commands {
    param([hashtable]$Info)

    Write-Host ""
    Write-Host "== $($Info.Name) build/run command ==" -ForegroundColor Cyan
    Write-Host $Info.Command
}

function Show-Practice {
    param([hashtable]$Info)

    Write-Host ""
    Write-Host "== $($Info.Name) practice prompts ==" -ForegroundColor Cyan
    for ($i = 0; $i -lt $Info.Practice.Count; $i++) {
        Write-Host ("{0}. {1}" -f ($i + 1), $Info.Practice[$i])
    }
}

function Invoke-Quiz {
    param([hashtable]$Info)

    Write-Host ""
    Write-Host "== $($Info.Name) quiz ==" -ForegroundColor Cyan
    $score = 0

    foreach ($question in $Info.Quiz) {
        $response = (Read-Host $question.Prompt).Trim()
        $normalized = $response.ToLowerInvariant()
        $accepted = @($question.Answers | ForEach-Object { $_.ToLowerInvariant() })

        if ($accepted -contains $normalized) {
            Write-Host "Correct!" -ForegroundColor Green
            $score++
        }
        else {
            Write-Host ("Not quite. Expected one of: {0}" -f ($question.Answers -join ", ")) -ForegroundColor Yellow
        }
    }

    Write-Host ("Score: {0}/{1}" -f $score, $Info.Quiz.Count) -ForegroundColor Cyan
}

function Invoke-Example {
    param([hashtable]$Info)

    Write-Host ""
    Write-Host "== running $($Info.Name) example ==" -ForegroundColor Cyan

    if (-not (Get-Command $Info.Tool -ErrorAction SilentlyContinue)) {
        Write-Host "$($Info.Tool) is not installed or not on PATH." -ForegroundColor Yellow
        return
    }

    $runSpec = Get-RunSpec -Name $Info.Name -ExamplePath $Info.Example

    try {
        & $runSpec.Tool @($runSpec.BuildArgs)
        if ($LASTEXITCODE -eq 0 -and $runSpec.OutputPath) {
            & $runSpec.OutputPath
        }
    }
    finally {
        if ($runSpec.OutputPath -and (Test-Path $runSpec.OutputPath)) {
            Remove-Item $runSpec.OutputPath -Force -ErrorAction SilentlyContinue
        }
    }
}

function Invoke-LanguageAction {
    param(
        [string]$SelectedLanguage,
        [string]$SelectedAction
    )

    $info = Get-LanguageInfo -Name $SelectedLanguage

    switch ($SelectedAction) {
        "Overview" { Show-Overview -Info $info }
        "Commands" { Show-Commands -Info $info }
        "Practice" { Show-Practice -Info $info }
        "Quiz"     { Invoke-Quiz -Info $info }
        "Run"      { Invoke-Example -Info $info }
    }
}

function Start-InteractiveMode {
    while ($true) {
        Write-Host ""
        Write-Host "== Lower-Level Language Learning Utility ==" -ForegroundColor Green
        Write-Host "1. Study C"
        Write-Host "2. Study C++"
        Write-Host "3. Study Rust"
        Write-Host "4. Study Go"
        Write-Host "Q. Quit"

        $selection = (Read-Host "Choose a language").Trim().ToUpperInvariant()

        switch ($selection) {
            "1" { $chosenLanguage = "C" }
            "2" { $chosenLanguage = "C++" }
            "3" { $chosenLanguage = "Rust" }
            "4" { $chosenLanguage = "Go" }
            "Q" { return }
            default {
                Write-Host "Unknown choice. Please select 1-4 or Q." -ForegroundColor Yellow
                continue
            }
        }

        while ($true) {
            Write-Host ""
            Write-Host "== $chosenLanguage menu ==" -ForegroundColor Green
            Write-Host "1. Overview"
            Write-Host "2. Build/run command"
            Write-Host "3. Practice prompts"
            Write-Host "4. Quiz"
            Write-Host "5. Run example"
            Write-Host "B. Back"

            $actionChoice = (Read-Host "Choose an action").Trim().ToUpperInvariant()

            switch ($actionChoice) {
                "1" { Invoke-LanguageAction -SelectedLanguage $chosenLanguage -SelectedAction "Overview" }
                "2" { Invoke-LanguageAction -SelectedLanguage $chosenLanguage -SelectedAction "Commands" }
                "3" { Invoke-LanguageAction -SelectedLanguage $chosenLanguage -SelectedAction "Practice" }
                "4" { Invoke-LanguageAction -SelectedLanguage $chosenLanguage -SelectedAction "Quiz" }
                "5" { Invoke-LanguageAction -SelectedLanguage $chosenLanguage -SelectedAction "Run" }
                "B" { break }
                default { Write-Host "Unknown choice. Please select 1-5 or B." -ForegroundColor Yellow }
            }
        }
    }
}

if ($PSBoundParameters.ContainsKey("Language") -and $PSBoundParameters.ContainsKey("Action")) {
    Invoke-LanguageAction -SelectedLanguage $Language -SelectedAction $Action
}
elseif ($PSBoundParameters.ContainsKey("Language") -or $PSBoundParameters.ContainsKey("Action")) {
    throw "Provide both -Language and -Action together, or omit both for interactive mode."
}
else {
    Start-InteractiveMode
}
