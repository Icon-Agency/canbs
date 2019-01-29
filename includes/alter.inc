<?php

/**
 * @file
 * hook_*_alter() functions in CanBS
 */

use Drupal\Core\Form\FormStateInterface;

function canbs_page_attachments_alter(&$page) {
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, shrink-to-fit=no',
    ),
  );
  $page['#attached']['html_head'][] = [$viewport, 'viewport'];
}

/**
 * Implements hook_theme_suggestions_HOOK_alter() for region templates.
 */
function canbs_theme_suggestions_region_alter(array &$suggestions, array $variables) {
  $nowrap = [
    'breadcrumb',
    'highlighted',
    'content',
    'primary_menu',
    'header',
    'sidebar_first',
    'sidebar_second',
  ];
  if ( theme_get_setting('canbs_region_clean_' . $variables['elements']['#region']) !== NULL) {
    $region_clean = theme_get_setting('canbs_region_clean_' . $variables['elements']['#region']);
  }
  else {
    $region_clean = in_array($variables['elements']['#region'], $nowrap);
  }
  if ( $region_clean ) {
    $suggestions[] = 'region__nowrap';
  }
}

/**
 * Implements hook_theme_HOOK_form_alter() for node add/edit.
 */
function canbs_form_node_form_alter(&$form, FormStateInterface $form_state) {
  $form['#theme'] = ['node_edit_form'];
}

/**
 * Implements hook_theme_suggestions_HOOK_alter() for links templates.
 */
function canbs_theme_suggestions_links_alter(array &$suggestions, array $variables) {
  // ToDo get from core a variable to flag inline links
  if ( in_array('inline', $variables['attributes']['class']) ) {
    $suggestions[] = 'links__inline';
  }
}

/**
 * Implements hook_theme_suggestions_HOOK_alter() for form templates.
 */
function canbs_theme_suggestions_form_alter(array &$suggestions, array $variables) {
  if ($variables['element']['#form_id'] == 'search_block_form') {
    $suggestions[] = 'form__search_block_form';
  }
  if ($variables['element']['#form_id'] == 'node_form') {
    $suggestions[] = 'node_edit_form';
  }
}

/**
 * Implements hook_theme_suggestions_HOOK_alter() for form templates.
 */
function canbs_theme_suggestions_input_alter(array &$suggestions, array $variables) {
  if ($variables['element']['#type'] == 'submit' && theme_get_setting('canbs_button')) {
    $suggestions[] = 'input__submit_button';
  }
}
/**
 * Implements hook_theme_suggestions_HOOK_alter().
 */
function canbs_theme_suggestions_block_alter(&$suggestions, $variables) {
  // Add block bundle theme suggestion
  $elements = $variables['elements'];

  if (isset($elements['content']['#block_content'])) {
    $bundle = $elements['content']['#block_content']->bundle();

    $new_suggestions = [];
    $flag_inserted = FALSE;

    foreach ($suggestions as $suggestion){
      $new_suggestions[] = $suggestion;
      if ($suggestion == 'block__block_content' && !$flag_inserted){
        $new_suggestions[] = "block__block_content__{$bundle}";
        $flag_inserted = TRUE;
      }
    }

    $suggestions = $new_suggestions;
  }
}

/**
 * Implements hook_form_alter() to add classes to the search form.

function canbs_form_alter(&$form, \Drupal\Core\Form\FormStateInterface $form_state, $form_id) {
if (in_array($form_id, ['search_block_form', 'search_form'])) {
$key = ($form_id == 'search_block_form') ? 'actions' : 'basic';
if (!isset($form[$key]['submit']['#attributes'])) {
$form[$key]['submit']['#attributes'] = new Attribute();
}
$outline = theme_get_setting('canbs_button_outline') ? 'btn-primary-outline' : 'btn-primary';
// If use the $outline variable, HTML crashes. Even if use the value directly
$form[$key]['submit']['#attributes']->addClass(array('btn', 'btn-primary', 'btn-sm'));
}
}
 */