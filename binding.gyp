{
    "targets": [
        {
            "target_name": "cppMod",
            "include_dirs": [
                "cpp",
                "<!(node -e \"require('nan')\")"
            ],
            "sources": []
        }
    ]
}