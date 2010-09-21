// $Id:$

/**
 * @file
 * Ooyala Javascript API implementation.
 *
 * The Ooyala javascript API allows you to register a callback function that
 * Ooyala will use to notify your script(s) of video player events. Since it is
 * only possible to specify a single callback function per player and we want
 * to make sure that any number of modules have access to the API we implement
 * a simple system that allows modules to register a listener which will be
 * notified whenever an event notification is recieved by the callback
 * function.
 */

/**
 * Intialize the Drupal.ooyala object if it hasn't already been initialized.
 *
 * Modules wishing to use the Drupal.ooyala API should include this line at the
 * top of their javascript files as it is possible for the module's javascript
 * to be included before this file.
 */
Drupal.ooyala = Drupal.ooyala || {'listeners': {}};

/**
 * Dispatch Ooyala player callback events to all registered listeners.
 *
 * Listeners are registered in the Drupal.ooyala.listeners object as follows.
 * @code
 *    Drupal.ooyala.listeners = function (player, eventName, p) {
 *      ...
 *    };
 * @endcode
 *
 * All listeners will recieve the following arguments.
 * - player: The DOM object representing the Ooyala player that is recieving
 *   the event notification.
 * - eventName: The event name as sent by Ooyala.
 * - p: Extra notification parameters.
 *
 * Read the Ooyala javascript API documentation for a complete list of events
 * and their possible extra notification parameters.
 * @link http://www.ooyala.com/support/docs/player_api#javascript
 */
Drupal.ooyala.notifyListeners = function(playerId, eventName, p) {
  var player = document.getElementById(playerId);
  jQuery.each(Drupal.ooyala.listeners, function() {
    this(player, eventName, p);
  });
};


Drupal.ooyala.listeners.ooyala = function(player, eventName, p) {
  var playerId = $(this).attr('id');

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

/**
 * Callback function for Ooyala Javascript API. Recieves events for the player
 * and then dispatches them so that other modules can interact with the player.
 *
 * @see Drupal.ooyala.notifyListeners()
 */
function receiveOoyalaEvent(playerId, eventName, p) {
  Drupal.ooyala.notifyListeners(playerId, eventName, p);
  return;
}
