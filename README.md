# Cleansheet CloudXR Client for Vision Pro

## Integrate with your tools
* CloudXRKit and deps are fetched/built as xcframeworks

1. In the main repo run: `./scripts/build.sh`
2. If the following error occurs: 
``` 
error: No gitlab access token in environment, so cannot access gitlab artifacts.
note: You need CXR_GITLAB_API_TOKEN set to a gitlab personal access token with 'api' scope permission.
```
See this [site](https://gitlab-master.nvidia.com/cloudxr/cloudxr-apple#per-user-settings)

Then in the terminal run: 
`export CXR_GITLAB_API_TOKEN="your token goes here"`
and repeat step `1.` 


* `import CloudXRKit` succeeds/builds/runs
* Gitlab CI enabled with lint, build, unit tests


## CI / Build

* `scripts/build.sh` — fetches dependencies, builds (Debug + Release), runs SwiftLint, and executes unit tests
* `scripts/test.sh` — runs unit tests only
* Build fails fast on first error (`set -e`)
