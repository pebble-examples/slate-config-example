Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 
  console.log('Showing configuration page: ' + url);
});

Pebble.addEventListener('webviewclosed', function(dict) {
  console.log('Received config data: ' + JSON.stringify(dict.payload));

});