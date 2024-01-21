
let sortByLikesButton = document.getElementById("sortByLikesButton");
sortByLikesButton.addEventListener("click", changeToLikesSort);

async function changeToLikesSort() {
    console.log("in change to likes main.js");
    const json = {method: "likes"};
    const response = await fetch(`/`, 
    {method: "POST", headers: {"Content-Type": "application/json"}, body: JSON.stringify(json)});
    location.reload()
    //console.log("sortmethod below in main.js");
    //console.log("clicked");
}

let sortByRecentButton = document.getElementById("sortByRecentButton");
sortByRecentButton.addEventListener("click", changeToRecentSort);

async function changeToRecentSort() {
    console.log("in change to recent main.js");
    const json = {method: "recent"};
    const response = await fetch(`/`, 
    {method: "POST", headers: {"Content-Type": "application/json"}, body: JSON.stringify(json)});
    location.reload()
    //console.log("sortmethod below in main.js");
    //console.log("clicked");
}

let likeButtonList = document.querySelectorAll(".likeButton");
for (let i = 0; i < likeButtonList.length; i++) {
    let button = likeButtonList[i];
    button.addEventListener("click", async () => {
        //console.log(button.id);
        let index = button.parentElement.children[3].innerText.indexOf(" ");
        let likes = Number(button.parentElement.children[3].innerText.substring(0, index));
        // let likes = Number(button.parentElement.children[3].innerText);
        button.parentElement.children[3].innerText = likes + 1 + " likes";
        incrementLikeCount(button.id).then();

    })
}

async function incrementLikeCount(id) {
    const json = {id: id};
    const response = await fetch("/like",
    {method: "POST", headers: {"Content-Type": "application/json"}, body: JSON.stringify(json)})
    return response;
}


let editPostLinks = document.querySelectorAll(".editPost");
for(let i = 0; i < editPostLinks.length; i++) {
    let link = editPostLinks[i];
    link.addEventListener("click", async () => {
        console.log(link.id);
        let data = {id: link.id};
        const response = await fetch("/update", 
        {method: "POST", headers: {"Content-Type": "application/json"}, body: JSON.stringify(data)})
        return response;
    })
}


const deleteButtonList = document.querySelectorAll(".deleteButton");
for (let i = 0; i < deleteButtonList.length; i++) {
    let postDeleteButton = deleteButtonList[i];
    postDeleteButton.addEventListener("click", async () => {
        const res = deletePostFromDatabase(Number(postDeleteButton.parentElement.id)).then(
            postDeleteButton.parentElement.remove()
        )
    })
}

async function deletePostFromDatabase(index) {
    const json = {id: index};
    const response = await fetch("/deletePost",
        {method: "DELETE", headers: {"Content-Type": "application/json"}, body: JSON.stringify(json)});
    return response;
    }
