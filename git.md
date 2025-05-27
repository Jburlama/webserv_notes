# Create a new branch

```bash
git branch branch_name
```

# Switch to branch

```bash
git switch -c branch_name

# or, the old way:
git checkout prime
```

# delete a brach

```bash
git branch -d branch_name
```

or for a remote branch:
```bash
git push origin --delete branch_name
```

# merge

we run the command inside the branch we want to merge to,
so if we want to merge `branch_name` into 'main', inside `main` we run:

```bash
git merge branch_name
```

# git log

```bash
git  log
```
Use git log to view the commits describe by a hash, and a commit message
'We should keep explicit log messages, so we can differenciate different logs'

If you want to see a commint in a branch you can use `git checktout` to a commit hash.
For convenience, you can refer to any commit or change within Git by using the first `7` characters of its hash.


# Reset to a commit

```bash
git reset --hard commit-hash
```
