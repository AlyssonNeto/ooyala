<?php
// $Id: ooyala_style_plugin.inc,v 1.1 2009/11/11 00:33:47 quicksketch Exp $

/**
 * @file
 * Provide the views Ooyala shared player object with default options and form.
 */

/**
 * Implementation of views_plugin_style().
 */
class ooyala_style_plugin extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();
    $options['playerwidth'] = array('default' => FALSE);
    $options['playerheight'] = array('default' => 1);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['playerwidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Player width'),
      '#default_value' => $this->options['playerwidth'],
    );
    $form['playerheight'] = array(
      '#type' => 'textfield',
      '#title' => t('Player height'),
      '#default_value' => $this->options['playerheight'],
    );

    $form['embededcode_field'] = array(
      '#type' => 'select',
      '#title' => t('Embeded code field'),
      '#description' => t('Select the field that contains the embeded code value for the video'),
      '#default_value' => $this->options['embededcode_field'],
      '#options' => $form['grouping']['#options'],
      '#required' => TRUE,
    );
  }
}
