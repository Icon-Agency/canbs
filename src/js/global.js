/**
 * @file
 * Global utilities.
 *
 */
(function ($, Drupal) {

  'use strict';

  Drupal.behaviors.canbs = {
    attach: function (context, settings) {

	$(window).scroll(function() {
        if ($(this).scrollTop() > 50){  
          $('body').addClass("scrolled");
        }
        else{
          $('body').removeClass("scrolled");
        }
      });

      var toggleAffix = function(affixElement, scrollElement, wrapper) {
  
        var height = affixElement.outerHeight(),
            top = wrapper.offset().top;
    
        if (scrollElement.scrollTop() >= top){
            wrapper.height(height);
            affixElement.addClass("affix");
        }
        else {
            affixElement.removeClass("affix");
            wrapper.height('auto');
        }
      
      };
  

      $('[data-toggle="affix"]').each(function() {
        var ele = $(this),
            wrapper = $('<div></div>');
    
        ele.before(wrapper);
        $(window).on('scroll resize', function() {
            toggleAffix(ele, $(this), wrapper);
        });
    
        // init
        toggleAffix(ele, $(window), wrapper);
      });

      $('#block-header-site-search-block .search-trigger').on('click', function () {
        $('#block-header-site-search-block .header-site-search-content-wrapper').addClass('open');
        $("#block-header-site-search-block .header-search-form .form-item-search input").focus();
      });

      $('#block-header-site-search-block .header-search-form .form-item-search input').blur(function () {
        $('#block-header-site-search-block .header-site-search-content-wrapper').removeClass('open');
      });

      // $('.search-underline input').on('blur', function () {
      //   $(this).parent('.search-underline').removeClass('active');
      // }).on('focus', function () {
      //   $(this).parent('.search-underline').addClass('active');
      // });

    }
  };

})(jQuery, Drupal);
