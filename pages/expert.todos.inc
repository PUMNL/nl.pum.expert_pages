<?php

function expert_pages_my_todo() {
  global $user;
  
  if (!civicrm_initialize()) {
    drupal_set_message(t('Could not initialize civicrm'), 'error');
    return;
  }
  
  $UFMatch = civicrm_api3('UFMatch', 'getsingle', array('uf_id' => $user->uid));
  $contact_id = $UFMatch['contact_id'];
  
  $og_id = civicrm_api3('OptionGroup', 'getvalue', array('return' => 'id', 'name' => 'activity_contacts'));
  $assignee_type_id = civicrm_api3('OptionValue', 'getvalue', array('return' => 'value', 'name' => 'Activity Assignees', 'option_group_id' => $og_id));
  $openActivityStatus = array(1); //scheduled
  
  $sql = "SELECT `customer`.`id` AS `customer_id`, `customer`.`display_name` AS `customer`, `civicrm_project`.`title` AS `project`, `civicrm_case`.`id` as `case_id`, `civicrm_activity`.`subject`, `civicrm_activity`.`activity_type_id` FROM `civicrm_activity_contact` INNER JOIN `civicrm_activity` ON `civicrm_activity_contact`.`activity_id` = `civicrm_activity`.`id` INNER JOIN `civicrm_case_activity` ON `civicrm_activity`.`id` = `civicrm_case_activity`.`activity_id` INNER JOIN `civicrm_case` ON `civicrm_case_activity`.`case_id` = `civicrm_case`.`id` LEFT JOIN `civicrm_case_project` ON `civicrm_case`.`id` = `civicrm_case_project`.`case_id` LEFT JOIN `civicrm_project` ON `civicrm_case_project`.`project_id` = `civicrm_project`.`id` LEFT JOIN `civicrm_contact` `customer` ON `civicrm_project`.`customer_id` = `customer`.`id` WHERE `civicrm_activity`.`status_id` IN (".implode(", ", $openActivityStatus).") AND `civicrm_activity_contact`.`contact_id` = ".$contact_id." AND `civicrm_activity_contact`.`record_type_id` = ".$assignee_type_id." ORDER BY `civicrm_activity`.`activity_date_time`";
  $dao = CRM_Core_DAO::executeQuery($sql);
  
  $header = array('Project', 'Customer', 'To do');
  $rows = array();
  while($dao->fetch()) {
    $row = array();
    $row[] = $dao->project;
    $row[] = $dao->customer;
    $row[] = $dao->subject;
    
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'my-expert-todos')));
}