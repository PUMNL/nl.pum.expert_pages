<?php

/**
 * Returns the HTML for a dashboard block
 * 
 * @param $variables
 *   An associative array containing
 *    - blocks: array of html code to be placed into a dashboard block
 * 
 * @ingroup themeable
 */
function theme_expert_pages_dashboard($variables) {
  $blocks = $variables['blocks'];
  $output = '';
  
  drupal_add_css(drupal_get_path('module', 'expert_pages') . '/css/dashboard.css');
  
  foreach($blocks as $block) {
   $output .= theme('expert_pages_dashboard_block', array('block' => $block));
  }
  
  $wrapper = theme('container', array(
    'element' => array(
      '#attributes' => array ( 'class' => array('expert-pages-dashboard-wrapper', 'sidebar')),
      '#children' => $output,
    )
  ));
  
  return $wrapper;
}

/**
 * Returns the HTML for a dashboard block
 * 
 * @param $variables
 *   An associative array containing
 *    - blocks: array of html code to be placed into a dashboard block
 * 
 * @ingroup themeable
 */
function theme_expert_pages_dashboard_block($variables) {
  $block = $variables['block'];
  $output = '';
  if (!empty($block['subject'])) {
    $output = '<h2>'.$block['subject'].'</h2>';
  }
  
  $output .= $block['content'];
  
  if (!empty($block['change_link'])) {
    $output .= theme('container', array('element' => array(
      '#attributes' => array('class' => array('change-link')),
      '#children' => $block['change_link'],
    )));
  }
  
  return theme('container', array(
     'element' => array(
       '#attributes' => array('class' => array('expert-pages-dashboard-block', 'block')),
       '#children' => $output,
     )
   )); 
}

