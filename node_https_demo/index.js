var https = require('https');  
var fs = require('fs');  
  
https.createServer({  
        key : fs.readFileSync('./privatekey.pem'),  
        cert : fs.readFileSync('./certificate.pem')  
},function(req,res){  
}).listen(8443);  
