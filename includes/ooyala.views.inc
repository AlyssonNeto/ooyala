<?php
// $Id: ooyala.views.inc,v 1.2 2010/02/22 23:06:12 heronog Exp $

/**
 * @file
 * Views integration for the Ooyala module.
 */

/**
  * Implementation of hook_views_plugin().
  */
function ooyala_views_plugins() {
  return array(
    'module' => 'ooyala',
    'style' => array(
      'ooyala_sharedplayer' => array(
        'title' => t('Ooyala Shared player'),
        'theme' => 'ooyala_sharedplayer_view',
        'theme path' => drupal_get_path('module', 'ooyala') . '/includes',
        'path' => drupal_get_path('module', 'ooyala') . '/includes',
        'help' => t('Show a big video player followed by the resulting nodes of this view. The nodes will play on the same player when clicked. This view requires that the CCK field that contains the embeded code is formatted as any of the "in shared player" options.'),
        'handler' => 'ooyala_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
