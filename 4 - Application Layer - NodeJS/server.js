var express = require('express'),
    app = express();

app.use(express.static(__dirname + '/www'));

app.listen(8000);

console.log('Server running on port 8000');
