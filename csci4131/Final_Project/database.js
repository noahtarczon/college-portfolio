
const mysql = require(`mysql-await`); // npm install mysql-await


// NEED TO SET UP TUNNELING !!!!!!!!!


var connPool = mysql.createPool({
connectionLimit: 5, // it's a shared resource, let's not go nuts.
host: "localhost",// this will work
user: "C4131F23U208",
database: "C4131F23U208",
password: "43314", // we really shouldn't be saving this here long-term -- and I probably shouldn't be sharing it with you...
});
// later you can use connPool.awaitQuery(query, data) -- it will return a promise for the query results.

// get all users
async function getUsers() {
    return await connPool.awaitQuery("SELECT * FROM Users;")

}
// returns the currently signed in user??? how???
async function getCurrentUser() {
    return await connPool.awaitQuery("SELECT * FROM Users WHERE logged_in = 1;")
}

// logs out the active user
async function logoutUser() {
    return await connPool.awaitQuery("UPDATE Users SET logged_in = 0 WHERE logged_in = 1;")
}

// logs in the user after checking credentials
async function loginUser(id) {
    return await connPool.awaitQuery("UPDATE Users SET logged_in = 1 WHERE user_id = ?", [id])
}

// checks to see if the user is in the database
async function isValidUser(username) {
    let user = await connPool.awaitQuery("SELECT * FROM Users WHERE username=?;", [username])
    return user;
}

// adds a user to the database
// logs them in as well
async function addUser(username, password) {
    return await connPool.awaitQuery("INSERT INTO Users (username, password, logged_in) VALUES (?, ?, 1);", [username, password])
}

// create a post 
async function addPost(creator, content) {
    return await connPool.awaitQuery("INSERT INTO Posts (username, content) VALUES (?, ?);", [creator, content])
}

// finds a certain post 
async function findPost(id) {
    return await connPool.awaitQuery("SELECT * FROM Posts WHERE post_id = ?;", [id])
}

// updates the likes on a post (MIGHT NEED FIXING)
async function likePost(id) {
    return await connPool.awaitQuery("UPDATE Posts SET likes = likes+1 WHERE post_id = ?;", [id])
}

// deletes a post with the passed in id
async function deletePost(id) {
    return await connPool.awaitQuery("DELETE FROM Posts WHERE post_id = ?", [id])
}

// edits a post 
async function editPost(content, id) {
    return await connPool.awaitQuery("UPDATE Posts SET content = ? WHERE post_id = ?;", [content, id])
}

// sort posts by recency
async function sortByRecent() {
    return await connPool.awaitQuery("SELECT * FROM Posts ORDER BY post_id DESC;")
}

// sort by likes
async function sortByLikes() {
    return await connPool.awaitQuery("SELECT * FROM Posts ORDER BY likes DESC;")
}

// get all posts
async function getPosts() {
    return await connPool.awaitQuery("SELECT * FROM Posts;")
}



module.exports = { 
    getUsers, 
    getCurrentUser, 
    logoutUser, 
    loginUser, 
    isValidUser, 
    addUser, 
    addPost, 
    findPost, 
    likePost, 
    deletePost, 
    editPost, 
    sortByRecent, 
    sortByLikes, 
    getPosts }
