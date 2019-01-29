<?php

/**
 * @file
 * hook_preprocess_HOOK() functions in CanBS
 */

use Drupal\Core\Template\Attribute;
use Drupal\Core\Link;

/**
 * Include helper functions
 */
include_once './' . drupal_get_path('theme', 'canbs') . '/includes/helper.inc';

/**
 * Implements hook_preprocess_HOOK() for HTML document templates.
 *
 * Adds body classes if certain regions have content.
 */
function canbs_preprocess_html(&$variables) {
  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['attributes']['class'][] = 'layout-two-sidebars';
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['attributes']['class'][] = 'layout-one-sidebar';
    $variables['attributes']['class'][] = 'layout-sidebar-first';
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['attributes']['class'][] = 'layout-one-sidebar';
    $variables['attributes']['class'][] = 'layout-sidebar-second';
  }
  else {
    $variables['attributes']['class'][] = 'layout-no-sidebars';
  }

  if (!empty($variables['page']['featured_top'])) {
    $variables['attributes']['class'][] = 'has-featured-top';
  }

  // Scroll Spy info added to body
  if ($scroll_spy = theme_get_setting('canbs_scroll_spy')) {
    $variables['attributes']['data-target'] = $scroll_spy;
    $variables['attributes']['data-spy'] = "scroll";
  }

  // Page specific info
  $variables['path_info']['args'] = FALSE;
  $path = \Drupal::service('path.current')->getPath();
  $path = ltrim($path, '/');
  $path_args = explode('/', $path);
  if (isset($path_args[0]) && isset($path_args[1]) && ($path_args[0] == 'node') && (is_numeric($path_args[1]))) {
    $variables['attributes']['class'][] = 'page-node-' . $path_args[1];
  }
  if (isset($path_args[0]) && isset($path_args[1]) && ($path_args[0] == 'user')) {
    $variables['attributes']['class'][] = 'page-user-' . $path_args[1];
  }

  // SEO Information
  $http_equiv = [
    '#type'       => 'html_tag',
    '#tag'        => 'meta',
    '#attributes' => [
      'http-equiv' => 'x-ua-compatible',
      'content'    => 'ie=edge',
    ],
  ];
  $variables['page']['#attached']['html_head'][] = [$http_equiv, 'http-equiv'];

}

/**
 * Implements hook_preprocess_HOOK() for page templates.
 */
function canbs_preprocess_page(&$variables) {
  if (\Drupal::moduleHandler()->moduleExists('bootstrap_library')) {
    $library = theme_get_setting('canbs_library');
    switch ($library) {
      case 'cdn' :
        $variables['#attached']['library'][] = 'bootstrap_library/bootstrap-cdn';
        break;
      case 'production':
        $variables['#attached']['library'][] = 'bootstrap_library/bootstrap';
        break;
      case 'development':
        $variables['#attached']['library'][] = 'bootstrap_library/bootstrap-dev';
        break;
    }
  }

  $fonts = theme_get_setting('canbs_google_fonts');
  if ($fonts) {
    $variables['#attached']['library'][] = 'canbs/' . $fonts;
  }

  $icons = theme_get_setting('canbs_icons');
  if ($icons) {
    $variables['#attached']['library'][] = 'canbs/' . $icons;
  }

  $system_messages = theme_get_setting('canbs_system_messages');
  if ($system_messages) {
    $variables['#attached']['library'][] = 'canbs/' . $system_messages;
  }

  //  $width = _canbs_content_width(!empty($variables['page']['sidebar_first']), !empty($variables['page']['sidebar_second']));
  //  $content_width = 'col-md-' . $width;
  $content_width = 'col';
  $sidebar_first_width = 'col-md-' . theme_get_setting('canbs_sidebar_first_width');
  $sidebar_second_width = 'col-md-' . theme_get_setting('canbs_sidebar_second_width');

  // Define default classes for containers
  $variables['navbar_top_attributes'] = [
    'class' => [
      'navbar',
    ],
    'id'    => ['navbar-top'],
  ];
  $variables['navbar_attributes'] = [
    'class' => [
      'navbar',
    ],
    'id'    => ['navbar-main'],
  ];
  $variables['content_attributes'] = [
    'class' => [
      'main-content',
      $content_width,
    ],
    'id'    => ['content'],
    'role'  => ['main'],
  ];
  $variables['sidebar_first_attributes'] = [
    'class' => [
      'sidebar',
      $sidebar_first_width,
    ],
    'id'    => ['sidebar_first'],
  ];
  $variables['sidebar_second_attributes'] = [
    'class' => [
      'sidebar',
      $sidebar_second_width,
    ],
    'id'    => ['sidebar_second'],
  ];

  // Affix
  $affix_navbar_top = theme_get_setting('canbs_navbar_top_affix');
  $affix_navbar = theme_get_setting('canbs_navbar_affix');
  $affix_sidebar_first = theme_get_setting('canbs_sidebar_first_affix');
  $affix_sidebar_second = theme_get_setting('canbs_sidebar_second_affix');
  if ($affix_navbar_top || $affix_navbar || $affix_sidebar_first || $affix_sidebar_second) {
    $variables['#attached']['library'][] = 'canbs/affix';
    if ($affix_navbar_top) {
      $variables['navbar_top_attributes']['data-toggle'] = 'affix';
    }
    if ($affix_navbar) {
      $variables['navbar_attributes']['data-toggle'] = 'affix';
    }
    if ($affix_sidebar_first) {
      $variables['sidebar_first_attributes']['data-toggle'] = 'affix';
    }
    if ($affix_sidebar_second) {
      $variables['sidebar_second_attributes']['data-toggle'] = 'affix';
    }
  }

  // Calculate widths
  if (theme_get_setting('canbs_content_offset')) {
    $variables['content_attributes']['class'][] = 'offset-md-' . theme_get_setting('canbs_content_offset');
  }
  if (theme_get_setting('canbs_sidebar_first_offset')) {
    $variables['sidebar_first_attributes']['class'][] = 'offset-md-' . theme_get_setting('canbs_sidebar_first_offset');
  }
  if (theme_get_setting('canbs_sidebar_second_offset')) {
    $variables['sidebar_second_attributes']['class'][] = 'offset-md-' . theme_get_setting('canbs_sidebar_second_offset');
  }

  // Define column layout classes
  switch (theme_get_setting('canbs_sidebar_position')) {
    case 'left':
      $variables['sidebar_first_attributes']['class'][] = 'order-first';
      $variables['content_attributes']['class'][] = 'order-last';
      break;
    case 'both':
      $variables['sidebar_first_attributes']['class'][] = 'order-first';
      $variables['sidebar_second_attributes']['class'][] = 'order-last';
      break;
    case 'right':
      $variables['content_attributes']['class'][] = 'order-first';
      $variables['sidebar_second_attributes']['class'][] = 'order-last';
      break;
  }

  // Define navigation position
  $variables['navbar_top_attributes']['class'][] = theme_get_setting('canbs_navbar_top_position');
  $variables['navbar_attributes']['class'][] = theme_get_setting('canbs_navbar_position');

  // Define navigation color scheme
  $variables['navbar_top_attributes']['class'][] = theme_get_setting('canbs_navbar_top_color');
  $variables['navbar_attributes']['class'][] = theme_get_setting('canbs_navbar_color');

  // Define navigation background classes
  $variables['navbar_top_attributes']['class'][] = theme_get_setting('canbs_navbar_top_background');
  $variables['navbar_attributes']['class'][] = theme_get_setting('canbs_navbar_background');

  // Define navigation toggle size
  switch (theme_get_setting('canbs_navbar_toggle')) {
    case 'navbar-toggleable-lg':
      $variables['navbar_attributes']['class'][] = 'navbar-expand-lg';
      break;
    case 'navbar-toggleable-md':
      $variables['navbar_attributes']['class'][] = 'navbar-expand-md';
      break;
    case 'navbar-toggleable-sm':
      $variables['navbar_attributes']['class'][] = 'navbar-expand-sm';
      break;
    case 'navbar-toggleable-xs':
      $variables['navbar_attributes']['class'][] = 'navbar-expand-xs';
      break;
    default:
      $variables['navbar_attributes']['class'][] = 'navbar-expand-md';
      break;
  }

  if (theme_get_setting('canbs_sidebar_collapse')) {
    $variables['content_attributes']['class'][] = 'sidebar-offcanvas';
    $variables['sidebar_collapse'] = TRUE;
    $variables['#attached']['library'][] = 'canbs/sidebar_collapse';
  }

  // Declare region attributes
  $variables['navbar_top_attributes'] = new Attribute($variables['navbar_top_attributes']);
  $variables['navbar_attributes'] = new Attribute($variables['navbar_attributes']);
  $variables['content_attributes'] = new Attribute($variables['content_attributes']);
  $variables['sidebar_first_attributes'] = new Attribute($variables['sidebar_first_attributes']);
  $variables['sidebar_second_attributes'] = new Attribute($variables['sidebar_second_attributes']);

  // Declare container layout
  $variables['container_navbar'] = theme_get_setting('canbs_navbar_container');
  $variables['container'] = theme_get_setting('canbs_fluid_container') ? 'container-fluid' : 'container';

  // Set no container for landing pages
  if (isset($variables['node'])
      && in_array($variables['node']->getType(),
      [
        'home_page',
        'pavilion_landing_page',]
      ) ){
    $variables['container'] = NULL;
  }

}

/**
 * Implements hook_preprocess_HOOK() for page templates.
 */
function canbs_preprocess_page_title(&$variables) {
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render
    // elements.
    $variables['title_prefix']['shortcut_wrapper'] = [
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    ];
    $variables['title_suffix']['shortcut_wrapper'] = [
      '#markup' => '</div>',
      '#weight' => -99,
    ];
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
}

/**
 * Implements hook_preprocess_HOOK() for maintenance-page.html.twig.
 */
function canbs_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }

  // Bootstrap Barrio has custom styling for the maintenance page.
  $variables['#attached']['library'][] = 'canbs/maintenance_page';
}

/**
 * Implements hook_preprocess_HOOK() for node.html.twig.
 */
function canbs_preprocess_node(&$variables) {
  // Remove the "Add new comment" link on teasers or when the comment form is
  // displayed on the page.
  if ($variables['teaser'] || !empty($variables['content']['comments']['comment_form'])) {
    unset($variables['content']['links']['comment']['#links']['comment-add']);
  }
}

/**
 * Implements hook_preprocess_HOOK() for region.html.twig.
 */
function canbs_preprocess_region(&$variables) {
  $nowrap = [
    'breadcrumb',
    'highlighted',
    'content',
    'primary_menu',
    'header',
    'sidebar_first',
    'sidebar_second',
  ];
  if (theme_get_setting('canbs_region_class_' . $variables['elements']['#region']) !== NULL) {
    $variables['attributes']['class'][] = theme_get_setting('canbs_region_class_' . $variables['elements']['#region']);
  }
  elseif (!in_array($variables['elements']['#region'], $nowrap)) {
    $variables['attributes']['class'][] = 'row';
  }
}

/**
 * Implements hook_preprocess_HOOK() for block.html.twig.
 */
function canbs_preprocess_block(&$variables) {
  // Add a clearfix class to system branding blocks.
  if ($variables['plugin_id'] == 'system_branding_block') {
    $variables['attributes']['class'][] = 'clearfix';
  }

  if (isset($variables['content']['#block_content'])) {
    $bundle = $variables['content']['#block_content']->bundle();
    $variables['attributes']['class'][] = 'block-block-content-' . str_replace('_', '-', $bundle);
  }
}

function canbs_preprocess_form_element(&$variables) {
  if (isset($variables['label']['#title'])) {
    if (is_object($variables['label']['#title']) && !$variables['label']['#title'] instanceof \Drupal\Component\Render\MarkupInterface) {
      $variables['input_title'] = NULL;
    }
    elseif (is_object($variables['label']['#title'])) {
      $variables['input_title'] = $variables['label']['#title'];
    }
    else {
      $variables['input_title'] = $variables['label']['#title'];
    }
  }
  if (isset($variables['element']['#attributes'])) {
    $variables['input_attributes'] = new Attribute($variables['element']['#attributes']);
  }
}

/**
 * Implements hook_preprocess_HOOK() for menu.html.twig.
 */
function canbs_preprocess_menu(&$variables) {
  $variables['attributes']['class'][] = 'clearfix';
}

function canbs_preprocess_menu_local_task(&$variables) {
  $link = $variables['element']['#link'];
  $url = $link['url'];
  $options = $url->getOptions();
  $url->setOptions($options + $link['localized_options']);
  $variables['item'] = Link::fromTextAndUrl($link['title'], $url);
}

/**
 * Implements hook_preprocess_HOOK() for menu-local-action.html.twig.
 */

function canbs_preprocess_menu_local_action(&$variables) {
  $link = $variables['element']['#link'];
  $link += [
    'localized_options' => [],
  ];
  $link['localized_options']['attributes']['class'][] = 'btn';
  $link['localized_options']['attributes']['class'][] = 'btn-secondary';
  $link['localized_options']['set_active_class'] = TRUE;
  $variables['link'] = [
    '#type'    => 'link',
    '#title'   => $link['title'],
    '#options' => $link['localized_options'],
    '#url'     => $link['url'],
  ];
}

/**
 * Implements template_preprocess_table()
 */
function canbs_preprocess_table(&$variables) {
  $variables['table_class'] = theme_get_setting('canbs_table_style');
  $variables['hover'] = theme_get_setting('canbs_table_hover');
  $variables['thead_class'] = theme_get_setting('canbs_table_head');
}

/**
 * Implements template_preprocess_views_view_table()
 */
function canbs_preprocess_views_view_table(&$variables) {
  $variables['table_class'] = theme_get_setting('canbs_table_style');
  $variables['hover'] = theme_get_setting('canbs_table_hover');
  $variables['thead_class'] = theme_get_setting('canbs_table_head');
}

/**
 * Implements hook_preprocess_input() to add classes to inputs.
 */
function canbs_preprocess_input(&$variables) {
  if (isset($variables['attributes']['class'])) {
    if (is_object($variables['attributes']['class'])) {
      if (in_array('error', $variables['attributes']['class']->value())) {
        $variables['attributes']['class']->addClass('is-invalid');
      }
    }
    elseif (is_array($variables['attributes']['class'])) {
      if (in_array('error', $variables['attributes']['class'])) {
        $variables['attributes']['class'][] = 'is-invalid';
      }
    }
  }
  if ($variables['element']['#type'] == 'submit') {
    $button_size = theme_get_setting('canbs_button_size');
    if (is_object($variables['attributes']['class'])) {
      if (empty(array_intersect([
        'btn-primary',
        'btn-secondary',
        'btn-success',
        'btn-warning',
        'btn-danger',
        'btn-link',
      ], $variables['attributes']['class']->value()))) {
        if (theme_get_setting('canbs_button_outline')) {
          $variables['attributes']['class']->addClass('btn-primary-outline');
        }
        else {
          $variables['attributes']['class']->addClass('btn-primary');
        }
      }
      if (!in_array('btn', $variables['attributes']['class']->value())) {
        $variables['attributes']['class']->addClass('btn');
      }
      if ($button_size) {
        if (empty(array_intersect([
          'btn-sm',
          'btn-lg',
        ], $variables['attributes']['class']->value()))) {
          $variables['attributes']['class']->addClass($button_size);
        }
      }
    }
    else {
      $variables['attributes']['class'][] = 'btn';
      if (theme_get_setting('canbs_button_outline')) {
        $variables['attributes']['class'][] = 'btn-primary-outline';
      }
      else {
        $variables['attributes']['class'][] = 'btn-primary';
      }
      if ($button_size) {
        $variables['attributes']['class'][] = $button_size;
      }
      $variables['attributes'] = new Attribute($variables['attributes']);
    }
  }
}

/**
 * Prepares variables for paragraph templates.
 *
 * Default template: paragraph.html.twig.
 *
 * Most themes use their own copy of paragraph.html.twig. The default is located
 * inside "templates/paragraph.html.twig". Look in there for the
 * full list of variables.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An array of elements to display in view mode.
 *   - paragraph: The paragraph object.
 *   - view_mode: View mode; e.g., 'full', 'teaser'...
 */
function canbs_preprocess_paragraph(&$variables) {

  // _template_preprocess_paragraph__{paragraph_type}__{view mode}
  $possible_hook_name = '_' . __FUNCTION__ . '__' . $variables['paragraph']->getParagraphType()->id  . '__' . $variables['view_mode'];

  if (function_exists($possible_hook_name)) {
    $possible_hook_name($variables);
  }else{
    // _template_preprocess_node__{content_type}
    $possible_hook_name = '_' . __FUNCTION__ . '__' . $variables['paragraph']->getParagraphType()->id;

    if (function_exists($possible_hook_name)) {
      $possible_hook_name($variables);
    }
  }
}

/*********************************Internal called functions ****************************************/


/**
 * Called by canbs_preprocess_paragraph()
 */
//function _canbs_preprocess_paragraph__top_slider_banner__default(&$variables){
//
//}