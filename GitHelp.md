# Git basics

## setting up git

Download 
[Git desktop program](https://git-scm.com/downloads)

[Set up an ssh keys](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

Setting up an ssh key is how you authenticate with github.  The following are taken from the above link.  Open `git bash` and use these commands there.:

`ssh-keygen -t ed25519 -C "your_email@example.com"`

`eval "$(ssh-agent -s)"`

`ssh-add ~/.ssh/id_ed25519`

These steps create an ssh key.  Now you have to upload it to github.  To see your key use the following command:

`cat ~/.ssh/id_ed25519.pub`

Copy everything that command outputs.  In github, go to you setings (click on your profile in the upper right corner) > SSH and GPG keys > new ssh key.  Here you can paste in what you copied along with a title (doesn't matter).

![image](https://github.com/mikeywilly19/BYUSpaceCraftClub/assets/99697494/8657b979-8aba-4880-8e45-1c5208f6be46)


Now you should be authenticated with github.  Now go to the project page for pocket cube.  There is a green button that says `code`.  Click on that and copy the ssh address.

![](images/github_sshaddress.png)

Navigate to were you want to put the project directory and use the command `git clone` and the address you just copied.  This should clone the repository to your computer.

## Contributing with git

Git makes working on group projects convinient.  However, we should avoid working on the same part of code at the same time.  Doing so will create a merge conflict and someone will have to tell it which version to accept.

The following commands are useful for working in git:

`git pull`
* Pulls what's online to your computer

`git status`
* Shows which files you have changed since your last commit.  Files in red are changes you've made.  Files in green are staged for a commit.

`git add *`
* Adds all files you changed to the staging area.  These are changes you want to synchronyze with everyone.  Instead of the astrix you could specity individual files.

`git commit -m "Your message"`
* Everything that was staged will be get commited to the project.  Run `git status` again before to make sure you're commiting the right files

`git push`
* This will send your commits to the pocketcube repository so everyone can access them.
