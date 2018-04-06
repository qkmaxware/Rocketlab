{
    "targets": [
        {
            "target_name": "rocketcore",
            "include_dirs": [
                "cpp",
                "<!(node -e \"require('nan')\")"
            ],
            "sources": [
                "cpp/definition.cpp",
                "cpp/simulator.cpp",
                "cpp/nanconvert.cpp",
                "cpp/linearalgebra.cpp"
            ]
        }
    ]
}