<?php
// $Id:$

/**
 * @file
 * ooyala_handler_field_status.inc
 *
 * Provides a handler for displaying video length column.
 */
class ooyala_handler_field_status extends views_handler_field_node {
  function render($values) {
    $values = drupal_clone($values); // Prevent affecting the original.
    $status = $values->{$this->field_alias};
    return theme('ooyala_video_status', $status, FALSE);
  }
}
