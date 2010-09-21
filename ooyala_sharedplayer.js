// $Id: ooyala_sharedplayer.js,v 1.3 2010/02/24 00:47:07 heronog Exp $

/**
 * @file
 * JavaScript for the Views Ooyala shared player output.
 */
setTimeout('', 1); //does the timeout work?
jQuery.each(Drupal.settings.ooyalaSharedPlayerCodes, function(i, val) {
  $('#' + Drupal.settings.ooyalaSharedPlayer + "-" + i).click(function() {
    document.getElementById(Drupal.settings.ooyalaSharedPlayer).setQueryStringParameters({embedCode: val});
  });
});

// If the ooyala_player.js hasn't been loaded yet we initialize the object so
// that we can added to it.
Drupal.ooyala = Drupal.ooyala || {'listeners': {}};

/**
 * Add an event responder to Drupal.ooyala.listners list.
 */
Drupal.ooyala.listeners.shared_player = function(player, eventName, p) {
  playerId = $(this).attr('id');
  switch(eventName) {
    case 'embedCodeChanged':
      $('#title-' + playerId ).empty().append(p.title);
      $('#description-' + playerId ).empty().append(p.description);
      break;
    case 'loadComplete':
      var description = document.getElementById(Drupal.settings.ooyalaSharedPlayer).getCurrentItem();
      $('#title-' + playerId ).empty().append(description.title);
      $('#description-' + playerId ).empty().append(description.description);     
      break;
  }
  return;
}
