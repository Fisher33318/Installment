# Command line instructions
## You can also upload existing files from your computer using the instructions below.


Git global setup<br />
`git config --global user.name "username"`<br />
`git config --global user.email "email"`<br />

1.  Create a new repository<br />
    ```
    git clone https://..................
    cd .......
    touch README.md
    git add README.md
    git commit -m "add README"
    git push -u origin master
    ```

2.  Push an existing folder<br />
    ```
    cd existing_folder
    git init
    git remote add origin https://.......................
    git add .
    git commit -m "Initial commit"
    git push -u origin master
    ```
    
不同電腦間須更新<br />
git pull 或 git pull origin master<br />

ref:<br />
https://ihower.tw/git/remote.html