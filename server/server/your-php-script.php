<?php
if (isset($_GET['url'])) {
    $url = $_GET['url'];
    // URL이 입력되었는지 확인하고, 입력된 URL로 리디렉션
    header("Location: " . $url);
    exit;
}
?>
<!DOCTYPE html>
<html>
<head>
    <title>Computer Network Assignment</title>
    <style>
        h1 {
            text-align: center;
        }
        .center-container {
            display: flex;
            justify-content: center;
            align-items: center;
        }
        #search-bar {
            display: flex;
            border: 1px solid #dfe1e5;
            border-radius: 24px;
            width: 600px;
            padding: 5px 10px;
        }
        #search-input {
            flex: 1;
            border: none;
            outline: none;
        }
        #search-button {
            background-color: #4285f4;
            color: #ffffff;
            border: none;
            border-radius: 24px;
            padding: 5px 10px;
            margin-left: 10px;
            cursor: pointer;
        }
        #footer {
            text-align: center;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <h1>Hello, this is a simple web page!</h1>
    <div class="center-container">
        <form action="your-php-script.php" method="GET">
            <div id="search-bar">
                <input type="text" name="url" placeholder="Enter URL" id="search-input">
                <input type="submit" value="Go" id="search-button">
            </div>
        </form>
    </div>
    <div id="footer">
        Network Assignment
    </div>
</body>
</html>
