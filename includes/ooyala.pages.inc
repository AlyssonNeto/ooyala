<?php
// $Id: ooyala.pages.inc,v 1.12 2010/08/11 00:22:48 quicksketch Exp $

/**
 * @file
 * Menu callbacks and page-specific functions for the Ooyala module.
 */

/**
 * Menu callback; Display global configuration options for Ooyala fields.
 */
function ooyala_settings_form() {
  $form['ooyala_global_pcode'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_global_pcode', ''),
    '#title' => t('Ooyala Partner code'),
    '#max_length' => 28,
    '#description' => t('Enter your Ooyala partner code. This code is 28 characters long'),
  );

  $form['ooyala_global_secret'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_global_secret', ''),
    '#title' => t('Ooyala Secret code'),
    '#max_length' => 40,
    '#description' => t('Enter your Ooyala secret code. This code is 40 characters long.'),
  );

  $form['ooyala_thumbnail_path'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_thumbnail_path', 'ooyalathumbs'),
    '#title' => t('Thumbnail file path'),
    '#description' => t('The file path where thumbnail images will be saved. Do not include trailing slash'),
    '#after_build' => array('ooyala_settings_path_validate'),
    '#validate' => array('ooyala_settings_path_validate'),
    '#field_prefix' => file_directory_path() . '/',
  );

  $form['ooyala_default_thumbnail_path'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('ooyala_default_thumbnail_path', FALSE),
    '#title' => t('Default thumbnail'),
    '#description' => t('If you want to use a default thumbnail, write its path here. This path is relative to the global files path to the site.'),
    '#after_build' => array('ooyala_settings_file_validate'),
    '#validate' => array('ooyala_settings_file_validate'),
    '#field_prefix' => file_directory_path() . '/',
  );

  $form['ooyala_video_height'] = array(
    '#type' => 'textfield', 
    '#description' => t('The height that the full player video will have'),
    '#title' => t('Full video height'),
    '#default_value' => variable_get('ooyala_video_height', 300),
  );

  $form['ooyala_video_width'] = array(
    '#type' => 'textfield', 
    '#description' => t('The width that the full player video will have'),
    '#title' => t('Full video width'),
    '#default_value' => variable_get('ooyala_video_width', 400),
  );

  if (module_exists('thickbox')) {
    $form['ooyala_thickbox_height'] = array(
      '#type' => 'textfield', 
      '#description' => t('The height that the thickbox window will have when showing full player video'),
      '#title' => t('Thickbox video height'),
      '#default_value' => variable_get('ooyala_thickbox_height', 400),
    );

    $form['ooyala_thickbox_width'] = array(
      '#type' => 'textfield', 
      '#description' => t('The width that the thickbox window will have when showing full player video'),
      '#title' => t('Thickbox video width'),
      '#default_value' => variable_get('ooyala_thickbox_width', 500),
    );
    
    $form['ooyala_thickbox_autoplay'] = array(
      '#type' => 'checkbox',
      '#description' => t('When the video is shown in a thickbox dialog you usually want it to play automatically, uncheck this if you don\'t want this behavior'),
      '#title' => t('Autoplay video in Thickbox'),
      '#default_value' => true,
    );
    
  }

  $form['ooyala_reporting_level'] = array(
    '#type' => 'radios',
    '#title' => t('Reporting level'),
    '#options' => array(
      '1' => t('Verbose - display errors and status messages'),
      '0' => t('Quiet - log errors to watchdog (recommended for production sites)'),
    ),
    '#default_value' => variable_get('ooyala_reporting_level', 1),
  );

  $autopublish_help = '';
  $autopublish_help .= t('Auto-publishing is a tool that you can use to publish content only after it has completed processing by the Ooyala servers. To set this up you need to do the following:');
  $autopublish_help .= theme('item_list', array(
    t('Visit your <a href="http://www.ooyala.com/backlot/web">Ooyala Backlot account</a>. Under the Account => Developers tab, there is an option for "API Ping URL". Enter the URL <strong>@url</strong>.', array('@url' => url('ooyala/ping', array('absolute' => TRUE)))),
    t('Visit the <a href="@content-types">content types overview</a>, and click "edit" on any types that contain Ooyala videos. Configure the type to set the default state to unpublished by unchecking the "Published" checkbox underneath the "Workflow settings" fieldset.', array('@content-types' => url('admin/content/types'))),
    t('Enable this option for auto-publishing (be sure to click the "Save configuration" button).'),
  ));

  $form['ooyala_autopublish'] = array(
    '#type' => 'fieldset',
    '#title' => t('Auto-publish content'),
    '#collapsible' => FALSE,
    '#description' => $autopublish_help,
  );

  $form['ooyala_autopublish']['ooyala_autopublish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-publish content when videos finish processing'),
    '#default_value' => variable_get('ooyala_autopublish', FALSE)
  );

  return system_settings_form($form);
}

/**
 * Element #validate function to ensure the entered directory is writable.
 */
function ooyala_settings_path_validate($element) {
  if (!empty($element['#value'])) {
    $path = file_create_path($element['#value']);
    if (!_ooyala_check_directory($path)) {
      form_error($element, t("The directory %path is not writable or could not be created. Check your server's file system permissions.", array('%path' => $path)));
    }
  }
  return $element;
}

/**
 * Element #validate function to ensure a file exists.
 */
function ooyala_settings_file_validate($element) {
  $path = file_create_path($element['#value']);
  if (!empty($element['#value']) && !is_file($path)) {
    form_error($element, t('The image at %path could not be found.', array('%path' => $path)));
  }
  return $element;
}

/**
 * Menu callback; Return AJAX data that will be sent to Ooyala on upload.
 */
function ooyala_upload_js($thisfield) {
  // TODO: We need a way here to enforce permissions. Right now the menu
  // callback is accesible to those who can 'access content'. What we need here
  // is to be able to identify which field is being validated and check for edit
  // permissions on it, as defined by content_permissions.module

  // The javascript from the form sends us the name of the field we are
  // validating in arg(2) in order to get the secret and pcode for the signature
  // from the widget settings.
  $fieldinfo = module_invoke('content', 'fields', $thisfield);

  $secret = variable_get('ooyala_global_secret', '');
  $pcode = variable_get('ooyala_global_pcode', '');

  module_load_include('inc', 'ooyala', 'includes/ooyala.partner_api');
  $ooyala = new OoyalaPartnerAPI($secret, $pcode);

  $unsets = array( // we will need to ignore some of the data from the request as it is not permanent or relative to ooyala
    'title', 'description', 'taxonomy'
  );

  $request = $_POST;

  // Ooyala says description, we say body... here we fix that.
  // Also we don't need to unset $request['body'] as it will be ignored.
  // See the $unsets array.
  $request['description'] = $request['body'];  
  $response['parameters'] = array();

  foreach($request as $name => $value) {
    // We will ignore the variables used for node creation.
    if (in_array($name,$unsets) ) {
      // We want to know if this is a CCK field.
      // Ask content.module if it knows something.
      $info = module_invoke('content', 'fields', $name); 
      if(!is_array($info)) {
        // It was not a cck field, it something else like taxonomy.
        // I want to deal with taxonomy as Ooyala supports tags.
        switch($name) {
          case 'taxonomy':
            // We will process taxonomy objects here.
            foreach($value as $voc_id => $assigned_tags) {
              if (in_array($voc_id, $fieldinfo['widget']['ooyala_tags'])){
                $voc = module_invoke('taxonomy', 'vocabulary_load', $voc_id);
                foreach ($assigned_tags as $tag_id) {
                  $termparents = module_invoke('taxonomy', 'get_parents', $tag_id);
                  // The first part of the path is always the vocabulary name.
                  foreach($termparents as $tagparent) {
                    $termpath[] = $tagparent->name;
                  }
                  $tag = module_invoke('taxonomy', 'get_term', $tag_id);
                  $termpath[] = $tag->name;

                  // This will only allow for one tag per vocabulary... is that
                  // required by Ooyala?
                  $response['parameters']['label['. $tag_id .']'] = '/' . variable_get('site_name', '') . '/' . $voc->name . '/' . join('/', $termpath);
                }
              }
            }
          break;
          case 'title':
          case 'description':
            $response[$name] = $value;
          break;
          default:
            $response['parameters'][$name] = $value;
          break;
        }
      }
    }
  }

  // We will add some housekeeping parameters here so we can later filter and
  // identify this videos better in Ooyala.
  $response['parameters']['signature'] = $ooyala->signed_params($response['parameters']);

  print drupal_to_js($response);
}

/**
 * Menu callback; Return AJAX data that will be sent to Ooyala on update.
 */
function ooyala_upload_metadata() {
  print drupal_to_js($_POST);
}

/**
 * Menu callback; Retrieve a new thumbnail from the Ooyala servers.
 */
function ooyala_refresh_thumbnail() {
  $token = drupal_get_token();
  $data = array();
  if (isset($_GET['token']) && $_GET['token'] == $token) {
    if (isset($_GET['embedcode']) && $_GET['embedcode']) {
      $embedcode = $_GET['embedcode'];
      // Try to retrieve a new thumbnail.
      if ($thumb_path = ooyala_api_fetch_image($embedcode)) {
        $data['content'] = theme('ooyala_upload_preview', $embedcode);
        $data['message'] = '';
        $data['error'] = 0;

        // Flush out all existing ImageCache images.
        if (function_exists('imagecache_image_flush')) {
          imagecache_image_flush($thumb_path);
        }
      }
      else {
        $data['content'] = theme('ooyala_upload_preview');
        $data['message'] = t('A thumbnail was not able to be retrieved. Check that the video code correct and that the video has finished processing.');
        $data['error'] = 0;
      }

      $data['content'] .= theme('ooyala_video_length', ooyala_api_video_property($embedcode, 'length'));
      $data['content'] .= theme('ooyala_video_status', ooyala_api_video_property($embedcode, 'status'));

    }
    else {
      $data['message'] = t('A video code must be provided before it can be previewed.');
      $data['error'] = 1;
    }
  }
  else {
    $data['message'] = t('Invalid token.');
    $data['error'] = 1;
  }

  drupal_json($data);
}

/**
 * Menu callback; Respond to Ooyala pings when a video has finished processing.
 *
 * This callback responds to the URL "ooyala/ping" within the Drupal site.
 * Typically this URL needs to be configured to receive pings from Ooyala.com
 * when a video has finished processing and is ready to be displayed. The
 * configuration for setting up a ping URL is at
 * https://www.ooyala.com/backlot/web, underneath the Account => Developers
 * tab. Enter a URL in the API Ping URL such as http://example.com/ooyala/ping,
 * if Drupal is installed at the base of the domain.
 */
function ooyala_ping() {
  $messages = array();
  $messages[] = t('Received ping from Ooyala Backlot.');

  if (variable_get('ooyala_autopublish', 0)) {
    if (isset($_GET['embedCode'])) {
      $embedcode = $_GET['embedCode'];
      $messages[] = t('Embed code received (@embedcode).', array('@embedcode' => $embedcode));

      if ($image_path = ooyala_api_fetch_image($embedcode)) {
        $messages[] = t('The video image has been downloaded to <a href="!url">@filename</a>.', array('!url' => file_create_url($image_path), '@filename' => basename($image_path)));
      }
      else {
        $messages[] = t('There was an error downloading the video image.');
      }

      if ($nodes = ooyala_load_nodes($embedcode)) {
        foreach ($nodes as $node) {
          if (!$node->status) {
            $node->status = 1;
            $a = array($node);
            _ooyala_sync_udpate_nodes($a);
            $messages[] = t('The content <a href="!url">%title</a> has published.', array('!url' => url('node/' . $node->nid), '%title' => $node->title));
          }
          else {
            $messages[] = t('The content <a href="!url">%title</a> was already published.', array('!url' => url('node/' . $node->nid), '%title' => $node->title));
          }
        }
      }
      else {
        $messages[] = t('Content matching the embed code was not found on the site. No action has been taken.');
      }
    }
    else {
      $messages[] = t('No embed code was passed in. No action has been taken.');
    }
  }
  elseif (user_access('administer site configuration')) {
    $messages[] = t('Ooyala auto-publishing is disabled. Visit the <a href="!url">Ooyala configuration</a> to enable support for pinging.', array('!url' => url('admin/settings/ooyala')));
  }

  // Disable the page cache for this request.
  if (function_exists('drupal_page_is_cacheable')) {
    // PressFlow or Drupal 7 mechanism:
    drupal_page_is_cacheable(FALSE);
  }
  else {
    // Drupal 6 cache disabling hack.
    $GLOBALS['conf']['cache'] = CACHE_DISABLED;
  }

  $output = implode("<br /> \n", $messages);
  watchdog('ooyala', $output);
  print $output;
}
