var prompt = require('prompt');

prompt.start();

prompt.get(['username', 'email'], function (err, result) {
	console.log('command-line input received:');
	console.log(' username: ' + result.username);
	console.log(' email: ' + result.email);
	});
