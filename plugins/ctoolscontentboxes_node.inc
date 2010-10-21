<?php
// $Id$

/**
 * Node as a block.
 */
class ctoolscontentboxes_node extends ctoolscontentboxes {

  /**
   * Implementation of boxes_content::__construct().
   */
  protected function __construct() {
    parent::__construct('node');
  }
}

/**
 * Validate the node selection.
 */
function boxes_ctools_node_content_type_edit_form_validate(&$form, &$form_state) {
  // This tricks the content type form validation.
  $form_id = $form_state['values']['form_id'];
  if ($form_id == 'boxes_add_form' || $form_id == 'boxes_box_form') {
    $form_state['op'] = 'add';
  }
  ctools_node_content_type_edit_form_validate($form, $form_state);
}
