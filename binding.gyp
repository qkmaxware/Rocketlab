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
                "cpp/simulator.cpp"
            ]
        }
    ]
}