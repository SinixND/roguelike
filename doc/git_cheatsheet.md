# Git
## Resources
[nvie.com - A successful git branching model](https://nvie.com/posts/a-successful-git-branching-model)
[dev.to - Beginner friendly git workflow for developers](https://dev.to/ajmal_hasan/beginner-friendly-git-workflow-for-developers-2g3g)

## Main branches (always existing)
### origin/master
Latest stable release branch 

### origin/develop
Latest development changes for next release to be merged to `origin/master`

## Supporting branches (temporary)
### feature
Develop new feature/functionality to merge into `origin/develop`

### release
Preparation of a release, should contain non-feature design changes (like directory structure changes and the like)?
