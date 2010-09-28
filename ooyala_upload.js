// $Id: ooyala_upload.js,v 1.6 2010/08/11 00:21:45 quicksketch Exp $

/**
 * @file
 * Ooyala upload widget JavaScript code.
 */

Drupal.ooyala = Drupal.ooyala || {};

Drupal.ooyala.onFileSelected = function(file) {
  $('.ooyala-button-container').removeClass('ooyala-finished');
  $('#ooyala-message').empty().append(Drupal.t('File selected: !filename. Click "Upload" to transfer to the video provider.', { '!filename': '<span class="ooyala-filename">' + Drupal.checkPlain(file.name) + '</span>' }));
}

Drupal.ooyala.onProgress = function(event) {
  if (event.ratio < 1) {
    $('#node-form input.form-submit, #node-form input.form-button').attr('disabled', 'disabled');
    $('#ooyala-message').empty().append(Drupal.t('Please wait while uploading... !percentage', { '!percentage': ('<span class="percentage">' + parseInt(event.ratio * 100) + '%</span>') }));
  }
}

Drupal.ooyala.onUploadComplete = function(event) {
  $('#ooyala-message').empty().append(Drupal.t('Upload complete.'));
  $('.ooyala-button-container').addClass('ooyala-finished');
  $('#node-form input.form-submit, #node-form input.form-button').attr('disabled', '');
}

Drupal.ooyala.onUploadError = function(text) {
  $('#ooyala-message').empty().append(Drupal.t('Upload Error: @error', { '@error': text }));
}

Drupal.ooyala.onEmbedCodeReady = function(embedCode) {
  $('.ooyala-embed-code-input').val(embedCode);
}

Drupal.ooyala.processUpload = function() {
  $(this).parent('.ooyala-button-container').find('input[type=text]').addClass('ooyala-embed-code-input');
  try { 
    var formvalues = $(this).parents('form').serializeArray();
    $.post(
      Drupal.settings.ooyalaUploadUrl,
      formvalues,
      function(data, textStatus) {
        ooyalaUploader.setTitle(data.title);
        ooyalaUploader.setParameters(data.parameters.signature);
          var errorText = ooyalaUploader.validate();

          if (errorText) {
            alert(errorText);
            return false;
          }
          ooyalaUploader.upload();
      },
      'json'
    );
  }
  catch(e) {
    alert(e);
  }
}

/**
 * AJAX success handler.
 */
Drupal.ooyala.refreshThumbnail = function(data) {
 $('.ooyala-preview').removeClass('ooyala-progress').find('.throbber').remove();

 if (!data['error']) {
   if (data['field_id']) {
     var preview = $('#' + data['field_id']).removeClass('ooyala-preview-hidden').get(0);
   }
   else {
     var preview = $('.ooyala-preview').removeClass('ooyala-preview-hidden').get(0);
   }

   $(preview).html(data['content']);

   var image = $('.ooyala-preview').find('img').get(0);
   if (image) {
     var src = $(image).attr('src');
     var timestamp = new Date().getTime();
     $(image).attr('src', src + '?' + timestamp);
   }
 }

 if (data['message']) {
   alert(data['message']);
 }
}

/**
* Add AJAX functionality to the thumbnail refresh link.
*/
Drupal.behaviors.ooyalaRefreshThumbnail = function(context) {
 $('a.ooyala-refresh', context).click(function() {
   var embedcode = $(this).parents('.ooyala-button-container').find('.ooyala-embed-code-input').val();
   var field_id = $(this).parents('.ooyala-field').find('.ooyala-preview').attr('id');
   $(this).parents('.ooyala-field').find('.ooyala-preview')
     .addClass('ooyala-progress')
     .append('<span class="throbber">&nbsp;</span>');
   $.ajax({
     url: Drupal.settings.ooyalaRefreshUrl,
     success: Drupal.ooyala.refreshThumbnail,
     dataType: 'json',
     data: { embedcode: embedcode, field_id: field_id }
   });
   return false;
 });
}

/**
 * This function is called by naming convention when the uploader finishes.
 */
function onOoyalaUploaderReady() {
  ooyalaUploader.addEventListener('fileSelected', 'Drupal.ooyala.onFileSelected');
  ooyalaUploader.addEventListener('progress', 'Drupal.ooyala.onProgress');
  ooyalaUploader.addEventListener('complete', 'Drupal.ooyala.onUploadComplete');
  ooyalaUploader.addEventListener('error', 'Drupal.ooyala.onUploadError');
  ooyalaUploader.addEventListener('embedCodeReady', 'Drupal.ooyala.onEmbedCodeReady');

  $('#ooyala-upload-button').attr('disabled', false).click(Drupal.ooyala.processUpload);
}
