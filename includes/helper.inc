<?php

/**
 * @file
 * helper functions
 */

use Drupal\media\Entity;

function _canbs_helper_get_media_field_url($field) {
  if (!isset($field['#items'])){
    return NULL;
  }

  $value = $field['#items']->getValue();
  if (empty($value[0]['target_id'])){
    return NULL;
  }

  //kint(Entity\Media::load($value[0]['target_id'])->get('field_media_image')->first()->getValue());

  return $value;
}