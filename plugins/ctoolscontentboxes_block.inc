<?php
// $Id$

/**
 * Blocks as a block.
 */
class ctoolscontentboxes_block extends ctoolscontentboxes {

  /**
   * Implementation of boxes_content::__construct().
   */
  protected function __construct() {
    parent::__construct('block');
  }
}
