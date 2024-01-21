
--TERMINAL SETUP--
1. open the Final_Project folder and run 'node tunnel.js' to start the tunnel
2. run 'node website.js' to start the server
(optionally) 
3. open the database using the command at the top of the schema.sql file, the password is in the database.js file 

--LOGGING IN--
1. Head to the login page
2. You can create a username up to 50 chars long 
3. Usernames cannot be copied (have to be unique) 
4. If the username is taken, you will be taken to another page to notify you that that account exists 

--LOGGING OUT--
1. Click the logout button on the menu to log out 

--POSTING-- 
1. A user must be logged in to post 
2. Every post can be edited, but only by the owner of the post 
3. Every post can be deleted, but only by the owner of the post 

--DEVELOPMENT CHOICES--
1. Users are logged in upon account creation
2. Previous users are logged out upon a new login
3. Editing a post does not make it more recent 
4. Users cannot like their own posts
5. Users can like a post as many times as they want
