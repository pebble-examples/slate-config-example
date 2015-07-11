Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://rawgit.com/pebble-examples/design-guides-config-example/dev/config/index.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var backgroundColor = configData['background_color'];

  // Send to watchapp
  Pebble.sendAppMessage({
    'KEY_COLOR_RED': parseInt(backgroundColor.substring(2, 4), 16),
    'KEY_COLOR_GREEN': parseInt(backgroundColor.substring(4, 6), 16),
    'KEY_COLOR_BLUE': parseInt(backgroundColor.substring(6), 16),
    'KEY_HIGH_CONTRAST': configData['high_contrast']
  }, function() {
    console.log('Send successful!');
  }, function() {
    console.log('Send failed!');
  });
});