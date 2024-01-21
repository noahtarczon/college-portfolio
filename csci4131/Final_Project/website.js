const port = 4132
const data = require("./database.js")
const exp = require("express")
const app = exp()

app.use(exp.urlencoded({ extended: true}));
app.use(exp.json());

app.set("view engine", "pug")
app.set("views", "resources/pugs")

app.use("/resources", exp.static("resources"))
app.use("/images", exp.static("resources/images"))
app.use("/pugs", exp.static("resources/pugs"))
app.use("/css", exp.static("resources/css"))
app.use("/js", exp.static("resources/js"))

var sortMethod = "recent";

///// GET Requests /////
app.get("/", (req, res) => {
    let page = parseInt(req.query.page ?? 1);   // default to 1
    if (!page) {   //catches NaN
        page = 1;
    }
    let offset = (page-1)*10; // 0 indexing

    data.getCurrentUser().then( currUser => {   // important to have who is logged in 
        let user = currUser[0];
        if (user) {
            user = user.username;
        }
        else if (!user) {
            user = "not signed in"
        }
        if (sortMethod == "likes") {
            data.sortByLikes().then( postPackets => {
                let posts = packetsToPosts(postPackets);
                let recentPosts = posts.slice(offset, offset+10);
                res.render("mainpage.pug", { recentPosts, user, page, sortMethod })    
            })
        }
        else if (sortMethod == "recent") {
            data.sortByRecent().then( postPackets => {
                let posts = packetsToPosts(postPackets);
                let recentPosts = posts.slice(offset, offset+10);
                res.render("mainpage.pug", { recentPosts, user, page, sortMethod })    
            })
        }
    })
})

function packetsToPosts(packets) {
    // helper to turn the packets into a list of posts for .get("/")
    let posts = [];
    if (packets) {
        for(let i = 0; i < packets.length; i++) {
            let postDetails = {};
            postDetails["creator"] = packets[i]["username"];
            postDetails["id"] = packets[i]["post_id"];
            postDetails["likes"] = packets[i]["likes"];
            postDetails["content"] = packets[i]["content"];
            posts.push(postDetails);
        }
    }
    return posts;
}

app.get("/login", (req, res) => {
    res.render("login.pug")
})

app.get("/logout", (req, res) => {
    res.render("logout.pug")
    data.logoutUser()
})

app.get("/edit:id", (req, res) => {
    let id = req.params.id.slice(1);
    data.getCurrentUser().then( userPacket => {
        let user = userPacket[0];
        if (user) {
            data.findPost(id).then( postPacket => {
                let post = postPacket[0];
                if (post["username"] === user["username"]) {
                    res.render("update.pug", { id, post });
                }
                else {
                    // someone is trying to edit a post they don't own
                    res.render("invalidUser.pug")
                }
            })
        }
        else {
            // no user is signed in
            res.render("invalidUser.pug")
        }
    })
})
/////-----------/////


///// DELETE Request /////
app.delete("/deletePost", (req, res) => { 
    // reference contactlog delete
    //console.log(req.body.id);
    data.getCurrentUser().then( result => {
        let user = result[0];
        console.log(user);
        if (user) {
            console.log(req.body.id);
            data.findPost(req.body.id).then( postPacket => {
                console.log(postPacket);
                let post = postPacket[0];
                if (post["username"] === user["username"]) {
                    data.deletePost(req.body.id).then(
                        res.send("Deleted Successfully")
                    )
                }
                else {
                    res.send("wrong account to delete this post")
                }
            })
        }
        else {
            res.send("no user signed in")
        }
    })

})
/////-----------/////


///// POST Requests /////
app.post("/login", (req, res) => {
    let body = req.body;
    //check if user is in database
    data.isValidUser(body.userName).then( result => {
        // result[0] is a RowDataPacket
        let user = result[0];
        let flag = 0;
        if (user) {    // account exists, check password
            data.logoutUser().then( () => {
                if (user.password === body.passWord) {
                    data.loginUser(user.user_id)
                }
                else {
                    // password is incorrect, set flag for later
                    flag = 1;
                }
            }).then( () => {
                if (flag) {
                    // wrong password, send to page to notify
                    res.render("incorrectPassword.pug")
                    
                }
                else {
                    res.redirect("/")
                }
            })
        }
        else {       // account doesn't exist
            data.logoutUser()
            .then( () => {
                data.addUser(body.userName, body.passWord).then(
                    res.render("registered.pug")
                )
            })
        }
    })
})

app.post("/", (req, res) => {
    // swaps the sorting method
    if (req.body.method == "recent") {
        sortMethod = "recent";
    }
    else if(req.body.method == "likes") {
        sortMethod = "likes";
    }
    res.status(200).send("changed sort method");
})

app.post("/like", (req, res) => {
    // like the post in the database
    data.likePost(req.body.id);
    res.send("liked post");
})

app.post("/postForm", (req, res) => {
    data.getCurrentUser().then( currUser => {
        let user = currUser[0];
        if (user == null) {
            res.redirect("/")
        }
        else {
            data.addPost(user["username"], req.body.userPost).then(res.redirect("/"));
        }
    })
})

app.post("/update", (req, res) => {
    data.editPost(req.body.postText, req.body.postID).then(
        res.redirect("/")
    )
})
/////-----------/////


///// JavaScript /////
app.get("/main.js", (req, res) => {
    res.status(200).send("main.js")
})

app.get("/login.js", (req, res) => {
    res.status(200).send("login.js")
})
/////-----------/////


app.listen(port, () => {
    console.log(`website is running on port ${port}, click here http://localhost:4132/`)
})

app.use((req, res, next) => {
    res.status(404).render("404.pug")
});
