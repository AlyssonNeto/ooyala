<?php
// $Id: ooyala.views.inc,v 1.2 2010/02/22 23:06:12 heronog Exp $

/**
 * @file
 * Views integration for the Ooyala module.
 */

/**
 * Implementation of hook_views_handlers().
 */
function ooyala_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ooyala') . '/includes',
    ),
    'handlers' => array(
      // field handlers
      'ooyala_handler_field_length' => array(
        'parent' => 'views_handler_field_node',
      ),
    ),
  );
}

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

/**
 * Return CCK Views data for the ooyala_field_settings($op == 'views data').
 */
function ooyala_views_content_field_data($field) {
  // Build the automatic views data provided for us by CCK.
  // This creates all the information necessary for the "url" field.
  $data = content_views_field_views_data($field);

  $db_info = content_database_info($field);
  $table_alias = content_views_tablename($field);
  $field_types = _content_field_types();

  // Add special handlers for length, and status fields.
  $data[$table_alias][$field['field_name'] .'_length']['field'] = array(
    'title' => $data[$table_alias][$field['field_name'] .'_length']['title'],
    'title short' => $data[$table_alias][$field['field_name'] .'_length']['title short'],
    'field' => $db_info['columns']['length']['column'],
    'table' => $db_info['table'],
    'handler' => 'ooyala_handler_field_length',
    'click sortable' => FALSE,
    'access callback' => 'content_access',
    'access arguments' => array('view', $field),
  );
  $data[$table_alias][$field['field_name'] .'_status']['field'] = array(
    'title' => $data[$table_alias][$field['field_name'] .'_status']['title'],
    'title short' => $data[$table_alias][$field['field_name'] .'_status']['title short'],
    'field' => $db_info['columns']['status']['column'],
    'table' => $db_info['table'],
    'handler' => 'views_handler_field_node',
    'click sortable' => FALSE,
    'access callback' => 'content_access',
    'access arguments' => array('view', $field),
  );

  return $data;
}