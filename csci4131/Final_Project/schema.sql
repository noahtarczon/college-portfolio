-- command to access my database
-- $ mysql -u C4131F23U208 C4131F23U208 -p -h cse-mysql-classes-01.cse.umn.edu

CREATE TABLE Users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(50) NOT NULL,
    logged_in BOOLEAN DEFAULT FALSE
    -- Other user-related fields
    -- users posts??
);

-- add a user to the database
INSERT INTO Users (username, password) VALUES ()

--check if the user exists in database
SELECT * FROM Users WHERE username=;

-- grabs the active user
SELECT * FROM Users WHERE logged_in = 1;

-- logs out active user
UPDATE Users SET logged_in = 0 WHERE logged_in = 1;

-- TESTING ONLY
DELETE FROM Users WHERE user_id = ;

CREATE TABLE Posts (
    post_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50),
    content VARCHAR(100),
    likes INT DEFAULT 0,
    FOREIGN KEY (username) REFERENCES Users(username)
);

-- ask chat how to write a query that can get only a certain users posts/liked posts

-- make a table to track the state of the posts
-- (i.e. how are they sorted)
-- only one item in the table and update based on what it currently is??

-- create a post
INSERT INTO Posts (username, content) VALUES ('name', 'This is a new post content.'); -- change values into variables passed into a js function

-- update a post
-- change content to 'new content' and post_id to 'whoever is logged in'
UPDATE Posts SET content = 'Updated content' WHERE post_id = 456; 

-- like a post
UPDATE Posts SET likes = likes+1 WHERE post_id = id;

-- get posts in recency order 
SELECT * FROM Posts ORDER BY post_id DESC;

-- CREATE TABLE Likes (
--     like_id INT PRIMARY KEY AUTO_INCREMENT,
--     user_id INT,
--     post_id INT,
--     FOREIGN KEY (user_id) REFERENCES Users(user_id),
--     FOREIGN KEY (post_id) REFERENCES Posts(post_id),
--     UNIQUE (user_id, post_id) -- Ensure a user can like a post only once
-- );

CREATE TABLE SortedBy (
    sortedBy text not null 
);

-- 