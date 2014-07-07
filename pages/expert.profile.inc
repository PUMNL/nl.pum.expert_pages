<?php

function expert_pages_my_profile() {
  
  $blocks = array();
  $blocks[] = _expert_pages_my_profile_personal_block();
  $blocks[] = _expert_pages_my_profile_ice();
  $blocks[] = _expert_pages_my_profile_communications();
  
  return theme('expert_pages_dashboard', array('blocks' => $blocks));
}

function _expert_pages_my_profile_personal_block() {
  global $user;
  
  if (!civicrm_initialize()) {
    drupal_set_message(t('Could not initialize civicrm'), 'error');
    return;
  }
  
  $UFMatch = civicrm_api3('UFMatch', 'getsingle', array('uf_id' => $user->uid));
  $contact_id = $UFMatch['contact_id'];
  
  $params['contact_id'] = $contact_id;
  $params['return.display_name'] = 1;
  $params['return.image_URL'] = 1;
  $params['return.street_address'] = 1;
  $params['return.postal_code'] = 1;
  $params['return.city'] = 1;
  $params['return.country'] = 1;
  
  $contact = civicrm_api3('Contact', 'getsingle', $params);
  
  $content = '';
  if (!empty($contact['image_URL'])) {
    $content .= theme('image', array('width' => '40', 'path' => $contact['image_URL'], 'alt' => $contact['display_name']));
  }
  
  $address = '';
  if (!empty($contact['street_address'])) {
    $address .= $contact['street_address'].'<br />';
  }
  $city = '';
  if (!empty($contact['postal_code'])) {
    $city .= $contact['postal_code'];
  }
  if (!empty($contact['city'])) {
    $city .= ' '.$contact['city'];
  }
  if (!empty($city)) {
    $address .= $city.'<br />';
  }
  if (!empty($contact['country'])) {
    $address .= $contact['country'].'<br />';
  }
  
  $content .= '<p>'.$address. '</p>';
  
  $block['content'] = $content;
  $block['subject'] = $contact['display_name'];
  $block['change_link'] = l(t('Change'), 'my-expert/profile');
  return $block;
}

function _expert_pages_my_profile_ice() {
  $content = '<p>hier komt ICE info</p>';
  
  $block['content'] = $content;
  $block['subject'] = t('In case of emergency');
  //$block['change_link'] = l(t('Change'), 'my-expert/profile');
  return $block;
}

function _expert_pages_my_profile_communications() {
  global $user;
  
  if (!civicrm_initialize()) {
    drupal_set_message(t('Could not initialize civicrm'), 'error');
    return;
  }
  
  $UFMatch = civicrm_api3('UFMatch', 'getsingle', array('uf_id' => $user->uid));
  $contact_id = $UFMatch['contact_id'];
  
  $params['contact_id'] = $contact_id;
  $params['return.phone'] = 1;
  $params['return.email'] = 1;
  
  $custom_field_id = false;
  //try to find the custom field for skype
  try {
    $custom_group_id = civicrm_api3('CustomGroup', 'getvalue', array('name' => 'Additional_Data', 'return' => 'id'));
    $custom_field_id = civicrm_api3('CustomField', 'getvalue', array('return' => 'id', 'name' => 'Skype_Name', 'custom_group_id' => $custom_group_id));
    $params['return.custom_'.$custom_field_id] = 1;
  } catch (Exception $ex) {
    //do nothoing
  }
  
  
  $contact = civicrm_api3('Contact', 'getsingle', $params);
  
  $content = '';
  if (!empty($contact['phone'])) {
    $content .= $contact['phone']. '<br>';
  }
  if (!empty($contact['email'])) {
    $content .= $contact['email']. '<br>';
  }
  if ($custom_field_id && !empty($contact['custom_'.$custom_field_id])) {
    $content .= $contact['custom_'.$custom_field_id]. '<br>';
  }
  
  $block['content'] = $content;
  $block['subject'] = t('Communication');
  $block['change_link'] = l(t('Change'), 'my-expert/profile');
  return $block;
}