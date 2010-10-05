<?php
// $Id: ooyala.partner_api.inc,v 1.1 2009/11/11 00:33:47 quicksketch Exp $

/**
 * @file
 * This file contains a class for signing requests to the Backlot
 */

class OoyalaPartnerAPI { 
  var $secret;
  var $pcode;

  function OoyalaPartnerAPI($secret, $pcode) {
    $this->secret = $secret;
    $this->pcode = $pcode;
  }

  function signed_params($params) { 

    if (!array_key_exists('expires', $params)) { 
      $params['expires'] = time() + 1000000;  // 15 minutes 
    } 

    $string_to_sign = $this->secret; 
    $param_string = 'pcode=' . $this->pcode; 
    $keys = array_keys($params); 
    sort($keys); 

    foreach ($keys as $key) { 
      $string_to_sign .= $key . '=' . $params[$key]; 
      $param_string .= '&' . rawurlencode($key) . '=' . rawurlencode($params[$key]); 
    } 

    $digest = hash('sha256', $string_to_sign, TRUE); 
    $signature = ereg_replace('=+$', '', trim(base64_encode($digest))); 
    return $param_string . '&signature=' . rawurlencode($signature);

  }
}