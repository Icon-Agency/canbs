!function(a,e){"use strict";Drupal.behaviors.canbs={attach:function(e,o){a(window).scroll(function(){50<a(this).scrollTop()?a("body").addClass("scrolled"):a("body").removeClass("scrolled")});var r=function(e,o,r){var a=e.outerHeight(),s=r.offset().top;o.scrollTop()>=s?(r.height(a),e.addClass("affix")):(e.removeClass("affix"),r.height("auto"))};a('[data-toggle="affix"]').each(function(){var e=a(this),o=a("<div></div>");e.before(o),a(window).on("scroll resize",function(){r(e,a(this),o)}),r(e,a(window),o)}),a("#block-header-site-search-block .search-trigger").on("click",function(){a("#block-header-site-search-block .header-site-search-content-wrapper").addClass("open"),a("#block-header-site-search-block .header-search-form .form-item input").focus()}),a("#block-header-site-search-block .header-search-form .form-item input").blur(function(){a("#block-header-site-search-block .header-site-search-content-wrapper").removeClass("open")})}}}(jQuery);